#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include <cstddef>
#include <limits>
#include <array>

namespace Ur::View {
    namespace Detail {
        template<typename T, std::size_t I>
        struct TTupleItem
        {
            using Type = T;
        };

        template<typename T, typename IndexSeq>
        struct TMakeTupleOfSize;

        template<typename T, std::size_t... Is>
        struct TMakeTupleOfSize<T, std::index_sequence<Is...>>
        {
            using Type = TTuple<typename TTupleItem<T, Is>::Type...>;
        };

        template<typename T, std::size_t N>
        struct TTupleOfSize
        {
            using Type = typename TMakeTupleOfSize<T, std::make_index_sequence<N>>::Type;
        };

        template<typename T, std::size_t N>
        using TTupleOfSize_T = typename TTupleOfSize<T, N>::Type;
    }

    template<typename TView, std::size_t N = 2>
    class TAdjacentView
        : public FView
        , public Detail::TMixins<TAdjacentView<TView, N>, TDefaultMixins>
        , public Detail::TConditionalMixins<TView::IsSized, TAdjacentView<TView, N>, TSizedMixins>
    {
        friend struct Ur::Cursor;

        static_assert(N > 0, "The 'window' size should be positive");

    public:
        using reference = Detail::TTupleOfSize_T<typename TView::reference, N>;
        using const_reference = Detail::TTupleOfSize_T<typename TView::const_reference, N>;
        using value_type = reference;

        struct Cursor
        {
            std::array<typename TView::Cursor, N> Nested;
        };
        struct ConstCursor
        {
            std::array<typename TView::ConstCursor, N> Nested;
        };
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = TView::IsSized;
        static constexpr bool LikeMap = false;

        template<typename UView>
        TAdjacentView(Misc::FFromViewTag, UView&& InView)
            : View(UR_FWD(InView))
        {
        }

        auto Num() const requires IsSized
        {
            auto UnderlyingSize = Ur::Size(View);
            using SizeT = decltype(UnderlyingSize);

            if (UnderlyingSize + 1 > N)
                return UnderlyingSize + 1 - SizeT(N);
            else
                return SizeT(0);
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            static_assert(std::is_lvalue_reference_v<typename TView::reference> || std::movable<typename TView::reference>,
                "We should be able to copy/move items coz we are reusing items from previous iteration by moving them into adjacent slots");

            std::array<TOptional<typename TView::reference>, N> AdjacentItems;
            int32 Counter = 0;

            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
                {
                    if (Counter < N)
                    {
                        AdjacentItems[Counter++].Emplace(UR_FWD(Item));
                    }
                    else
                    {
                        for (int32 Idx = 1; Idx < N; ++Idx)
                            AdjacentItems[Idx - 1] = MoveTemp(AdjacentItems[Idx]);

                        AdjacentItems[N - 1].Emplace(UR_FWD(Item));
                    }

                    if (Counter >= N)
                    {
                        return[&]<std::size_t... Is>(std::index_sequence<Is...>) {
                            return Callback(TReference<TSelf>(*AdjacentItems[Is]...));
                        }(std::make_index_sequence<N>{});
                    }
                    else
                    {
                        return Ur::ELoop::Continue;
                    }
                });
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            auto Curs = CursorEnd<IsForward>(Self);

            Curs.Nested[0] = Ur::Cursor::Begin<IsForward>(Self.View);

            for (std::size_t Idx = 1; Idx < N; ++Idx)
            {
                if (Ur::Cursor::IsEnd(Self.View, Curs.Nested[Idx - 1]))
                    return Curs;

                Curs.Nested[Idx] = Curs.Nested[Idx - 1];
                Ur::Cursor::Inc(Self.View, Curs.Nested[Idx]);
            }

            return Curs;
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            auto NestedEnd = Ur::Cursor::End<IsForward>(Self.View);

            return[&]<std::size_t... Is>(std::index_sequence<Is...>) {
                return TCursor<TSelf, IsForward>{ { (Is, NestedEnd)... } };
            }(std::make_index_sequence<N>{});
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            for (int32 Idx = 1; Idx < N; ++Idx)
                Curs.Nested[Idx - 1] = Curs.Nested[Idx];

            Ur::Cursor::Inc(Self.View, Curs.Nested.back());
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return[&]<std::size_t... Is>(std::index_sequence<Is...>) {
                return TReference<TSelf>(Ur::Cursor::Deref(Self.View, Curs.Nested[Is])...);
            }(std::make_index_sequence<N>{});
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return Ur::Cursor::Eq(Self.View, Lhs.Nested.back(), Rhs.Nested.back()) &&               // last elements are equal and...
                (
                    Ur::Cursor::IsEnd(Self.View, Lhs.Nested.back())                                 // ...they are the "end" elements (no need to check the rest)
                    ||                                                                              // ...or...
                    [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                        return (Ur::Cursor::Eq(Self.View, Lhs.Nested[Is], Rhs.Nested[Is]) && ...);  // ...the rest elements are equal (no need to check "end" elements here coz they are already checked)
                    }(std::make_index_sequence<N - 1>{})
                );
        }

    private:
        TView View;
    };


    template<typename TRng, std::size_t N = 2>
    auto Adjacent(TRng& Rng)
    {
        //TODO: own view & view in support
        return TAdjacentView<TRefView<TRng>, N>({}, TRefView<TRng>({}, Rng));
    }

    template<typename TRng>
    auto Pairwise(TRng& Rng)
    {
        //TODO: own view & view in support
        return TAdjacentView<TRefView<TRng>, 2>({}, TRefView<TRng>({}, Rng));
    }
}