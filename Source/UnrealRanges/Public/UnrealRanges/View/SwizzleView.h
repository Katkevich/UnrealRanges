#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    namespace Detail {
        template<typename UReference, std::size_t... ItemsOrder>
        struct TSwizzleReference
        {
            using Type = TTuple<Ur::TTupleElement_T<ItemsOrder, UReference>...>;
        };

        template<typename UReference, std::size_t... ItemsOrder>
        struct TSwizzleReference<UReference&, ItemsOrder...>
        {
            using Type = TTuple<std::add_lvalue_reference_t<Ur::TTupleElement_T<ItemsOrder, UReference>>...>;
        };

        template<typename UReference, std::size_t... ItemsOrder>
        struct TSwizzleReference<UReference&&, ItemsOrder...>
        {
            using Type = TTuple<Ur::TTupleElement_T<ItemsOrder, UReference>...>;
        };

        template<typename UReference, std::size_t... ItemsOrder>
        using TSwizzleReference_T = typename TSwizzleReference<UReference, ItemsOrder...>::Type;
    }

    template<Ur::RangeView TUnderlView, std::size_t... ItemsOrder>
    class TSwizzleView
        : public FView
        , public Detail::TMixins<TSwizzleView<TUnderlView, ItemsOrder...>, TDefaultMixins>
        , public Detail::TConditionalMixins<TUnderlView::IsSized, TSwizzleView<TUnderlView, ItemsOrder...>, TSizedMixins>
        , public Detail::TConditionalMixins<
            TIsPair_V<Detail::TSwizzleReference_T<typename TUnderlView::reference, ItemsOrder...>>,
            TSwizzleView<TUnderlView, ItemsOrder...>,
            TMapMixins>
    {
        friend struct Ur::Cursor;

        static_assert(Ur::TTupleSize_V<typename TUnderlView::value_type> == sizeof...(ItemsOrder),
            "the amount of indices you passed into swizzle should match the tuple size (members count is a case of an aggregate) of a view items");
        static_assert(Ur::View::Detail::SwizzleIndices<TUnderlView, ItemsOrder...>,
            "swizzle indices should be unique and be in the range of [0..N-1] where N is a tuple size of a view item");

    public:
        using reference = Detail::TSwizzleReference_T<typename TUnderlView::reference, ItemsOrder...>;
        using const_reference = Detail::TSwizzleReference_T<typename TUnderlView::const_reference, ItemsOrder...>;
        using value_type = reference;

        using Cursor = typename TUnderlView::Cursor;
        using ConstCursor = typename TUnderlView::ConstCursor;
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = TUnderlView::IsSized;
        static constexpr bool LikeMap = TIsPair_V<reference>;

        template<typename UUnderlView>
        TSwizzleView(Misc::FFromViewTag, UUnderlView&& InView)
            : View(UR_FWD(InView))
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            using std::get;

            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
                {
                    return Callback(TReference<TSelf>(get<ItemsOrder>(UR_FWD(Item))...));
                });
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return Ur::Cursor::Begin<IsForward>(Self.View);
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return Ur::Cursor::End<IsForward>(Self.View);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            return Ur::Cursor::Inc(Self.View, Curs);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static auto CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            using std::get;

            return [](auto&& Item) {
                return TReference<TSelf>(get<ItemsOrder>(UR_FWD(Item))...);
            }(Ur::Cursor::Deref(Self.View, Curs));
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return Ur::Cursor::Eq(Self.View, Lhs, Rhs);
        }

    private:
        TUnderlView View;
    };


    template<typename TRng, std::size_t... ItemsOrder>
    auto Swizzle(TRng& Rng)
    {
        static_assert(Detail::SwizzleIndices<TRng, ItemsOrder...>, "swizzle indices should be unique and be in the range of [0..N-1]");

        //TODO: own view & view in support
        return TSwizzleView<TRefView<TRng>, ItemsOrder...>({}, TRefView<TRng>({}, Rng));
    }
}