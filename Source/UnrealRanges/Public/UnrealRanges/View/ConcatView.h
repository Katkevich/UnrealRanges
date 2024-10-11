#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include "Templates/Tuple.h"
#include <variant>

namespace Ur::View {
    template<Ur::RangeView... TViews>
    class TConcatView
        : public FView
        , public Detail::TMixins<TConcatView<TViews...>, TDefaultMixins>
        , public Detail::TConditionalMixins<(TViews::IsBidir && ...), TConcatView<TViews...>, TBidirMixins>
        , public Detail::TConditionalMixins<(TViews::LikeMap && ...), TConcatView<TViews...>, TMapMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = std::common_reference_t<typename TViews::reference...>;
        using const_reference = std::common_reference_t<typename TViews::const_reference...>;
        using value_type = std::remove_cvref_t<reference>;
        using pointer = std::conditional_t<std::is_lvalue_reference_v<reference>, std::add_pointer_t<reference>, void>;

        static constexpr bool IsBidir = (TViews::IsBidir && ...);
        static constexpr bool IsSized = (TViews::IsSized && ...);
        static constexpr bool LikeMap = (TViews::LikeMap && ...);

        struct Cursor
        {
            TTuple<typename TViews::Cursor...> NestedCursors;
            int32 RngIndex = 0;
        };

        struct ConstCursor
        {
            TTuple<typename TViews::ConstCursor...> NestedCursors;
            int32 RngIndex = 0;
        };

        struct ReverseCursor
        {
            TTuple<typename TViews::ReverseCursor...> NestedCursors;
            int32 RngIndex = 0;
        };

        struct ReverseConstCursor
        {
            TTuple<typename TViews::ReverseConstCursor...> NestedCursors;
            int32 RngIndex = 0;
        };

    private:
        template<size_t Index>
        struct FIndex
        {};

    public:
        static_assert((!std::is_rvalue_reference_v<typename TViews::reference> && ...), "only lvalue references and values are supported (rvalues are messing common_reference)");
        static_assert((!std::is_rvalue_reference_v<typename TViews::const_reference> && ...), "only lvalue references and values are supported (rvalues are messing common_reference)");

        template<typename... UViews>
        TConcatView(Misc::FFromViewTag, UViews&&... InViews)
            : Views(UR_FWD(InViews)...)
        {
        }

        auto Num() const requires IsSized
        {
            return[&]<size_t... Indices>(std::index_sequence<Indices...>) {

                return (Ur::Size(get<Indices>(Views)) + ...);

            }(std::index_sequence_for<TViews...>{});
        }

    private:
        template<bool IsForward, size_t Index, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterationSubview(FIndex<Index>, TSelf& Self, TCallback Callback)
        {
            return Ur::Cursor::Iterate<IsForward>(At<Index, IsForward>(Self.Views), [&](auto&& Item)
                {
                    return Callback(UR_FWD(Item));
                });
        }

        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            return[&]<size_t... Indices>(std::index_sequence<Indices...>) {

                const bool bContinue = ((InternalIterationSubview<IsForward>(FIndex<Indices>{}, Self, Callback) == Ur::ELoop::Continue) && ...);
                return bContinue
                    ? Ur::ELoop::Continue
                    : Ur::ELoop::Break;

            }(std::index_sequence_for<TViews...>{});
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            TCursor<TSelf, IsForward> Curs = {
                .NestedCursors = ApplyTransform([](auto& View) { return Ur::Cursor::Begin<IsForward>(View); }, Self.Views),
                .RngIndex = 0
            };

            FastForward(Self, Curs);

            return Curs;
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            TCursor<TSelf, IsForward> Curs = {
                .NestedCursors = ApplyTransform([](auto& View) { return Ur::Cursor::End<IsForward>(View); }, Self.Views),
                .RngIndex = ViewsCount
            };

            return Curs;
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            ApplyAt<Ur::Cursor::IsForward<TSelf, TCursor>>(
                [&](auto& View, auto& NestedCursor) {
                    Ur::Cursor::Inc(View, NestedCursor);
                },
                ToCompTimeIndex(Curs.RngIndex),
                Self.Views,
                Curs.NestedCursors);

            FastForward(Self, Curs);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return ApplyAt<Ur::Cursor::IsForward<TSelf, TCursor>>(
                [&](auto& View, auto& NestedCursor) -> reference {
                    return Ur::Cursor::Deref(View, NestedCursor);
                },
                ToCompTimeIndex(Curs.RngIndex),
                Self.Views,
                Curs.NestedCursors);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return
                Lhs.RngIndex == Rhs.RngIndex &&
                (
                    Lhs.RngIndex == ViewsCount ||
                    ApplyAt<Ur::Cursor::IsForward<TSelf, TCursor>>(
                        [](auto& View, auto& LhsNested, auto& RhsNested) {
                            return Ur::Cursor::Eq(View, LhsNested, RhsNested);
                        },
                        ToCompTimeIndex(Lhs.RngIndex),
                        Self.Views,
                        Lhs.NestedCursors,
                        Rhs.NestedCursors)
                );
        }

    private:
        template<typename TSelf, typename TCursor>
        static void FastForward(TSelf& Self, TCursor& Curs)
        {
            if (Curs.RngIndex == ViewsCount)
                return;

            ApplyAt<Ur::Cursor::IsForward<TSelf, TCursor>>(
                [&](auto& View, auto& NestedCursor) {
                    if (Ur::Cursor::IsEnd(View, NestedCursor))
                    {
                        ++Curs.RngIndex;
                        FastForward(Self, Curs);
                    }
                },
                ToCompTimeIndex(Curs.RngIndex),
                Self.Views,
                Curs.NestedCursors);
        }

        template<typename UTuple>
        static auto ApplyTransform(auto Callback, UTuple& Tuple)
        {
            return[&]<size_t... Indices>(std::index_sequence<Indices...>) {
                return MakeTuple(Callback(get<Indices>(Tuple))...);
            }(std::index_sequence_for<TViews...>{});
        }

        template<bool IsForward, typename TIndex, typename... TTuples>
        static decltype(auto) ApplyAt(auto Callback, TIndex CompTimeIndex, TTuples&... Tuples)
        {
            return std::visit([&]<size_t Index>(FIndex<Index>) -> decltype(auto) {
                return Callback(At<Index, IsForward>(Tuples)...);
            }, CompTimeIndex);
        }

        static auto ToCompTimeIndex(int32 RuntimeIndex)
        {
            return[&]<size_t... Indices>(std::index_sequence<Indices...>) {

                std::variant<FIndex<Indices>...> CompTimeIndex;

                ((void)(RuntimeIndex == Indices && ((CompTimeIndex = FIndex<Indices>{}), true)), ...);

                return CompTimeIndex;
            }(std::index_sequence_for<TViews...>{});
        }

        template<size_t Index, bool IsForward, typename TTuple>
        static auto& At(TTuple& Tuple)
        {
            if constexpr (IsForward)
                return get<Index>(Tuple);
            else
                return get<ViewsCount - Index - 1>(Tuple);
        }

    private:
        TTuple<TViews...> Views;

        static constexpr size_t ViewsCount = sizeof...(TViews);
    };


    template<typename TRng, typename... TRngs>
    auto Concat(TRng& Rng, TRngs&... Rngs)
    {
        //TODO: own view & view in support
        return TConcatView<TRefView<TRng>, TRefView<TRngs>...>({}, TRefView<TRng>({}, Rng), TRefView<TRngs>({}, Rngs)...);
    }
}