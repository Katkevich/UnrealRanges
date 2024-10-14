#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {

    template<Ur::RangeView TUnderlView>
    class TReverseView
        : public FView
        , public Detail::TMixins<TReverseView<TUnderlView>, TDefaultMixins>
        , public Detail::TMixins<TReverseView<TUnderlView>, TBidirMixins>
        , public Detail::TConditionalMixins<TUnderlView::IsSized, TReverseView<TUnderlView>, TSizedMixins>
        , public Detail::TConditionalMixins<TUnderlView::LikeMap, TReverseView<TUnderlView>, TMapMixins>
    {
        friend struct Ur::Cursor;

    public:
        using value_type = typename TUnderlView::value_type;
        using reference = typename TUnderlView::reference;
        using const_reference = typename TUnderlView::const_reference;

        using Cursor = typename TUnderlView::ReverseCursor;
        using ConstCursor = typename TUnderlView::ReverseConstCursor;
        using ReverseCursor = typename TUnderlView::Cursor;
        using ReverseConstCursor = typename TUnderlView::ConstCursor;

        static constexpr bool IsBidir = true;
        static constexpr bool IsSized = TUnderlView::IsSized;
        static constexpr bool LikeMap = TUnderlView::LikeMap;

        template<typename UUnderlView>
        TReverseView(Misc::FFromViewTag, UUnderlView&& InView)
            : View(UR_FWD(InView))
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TFn>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TFn Fn)
        {
            return Ur::Cursor::Iterate<Misc::Opposite<IsForward>>(Self.View, Fn);
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return Ur::Cursor::Begin<Misc::Opposite<IsForward>>(Self.View);
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return Ur::Cursor::End<Misc::Opposite<IsForward>>(Self.View);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            return Ur::Cursor::Inc(Self.View, Curs);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return Ur::Cursor::Deref(Self.View, Curs);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return Ur::Cursor::Eq(Self.View, Lhs, Rhs);
        }

    private:
        TUnderlView View;
    };


    template<typename TRng>
    auto Reverse(TRng& Rng)
    {
        //TODO: own view & view in support
        return TReverseView<TRefView<TRng>>({}, TRefView<TRng>({}, Rng));
    }
}
