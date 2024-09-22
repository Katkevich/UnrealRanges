#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {

    template<typename TView>
    class TReverseView
        : public FView
        , public Detail::TMixins<TReverseView<TView>, TDefaultMixins>
        , public Detail::TMixins<TReverseView<TView>, TBidirMixins>
        , public Detail::TConditionalMixins<TView::IsSized, TReverseView<TView>, TSizedMixins>
    {
        friend struct Ur::Cursor;

    public:
        using value_type = typename TView::value_type;
        using reference = typename TView::reference;
        using const_reference = typename TView::const_reference;

        using Cursor = typename TView::ReverseCursor;
        using ConstCursor = typename TView::ReverseConstCursor;
        using ReverseCursor = typename TView::Cursor;
        using ReverseConstCursor = typename TView::ConstCursor;

        static constexpr bool IsBidir = true;
        static constexpr bool IsSized = TView::IsSized;

        template<typename UView>
        TReverseView(Misc::FFromViewTag, UView&& InView)
            : View(UR_FWD(InView))
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TFn>
        UR_DEBUG_NOINLINE static Misc::ELoop InternalIterate(TSelf& Self, TFn Fn)
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
        TView View;
    };


    template<typename TRng>
    auto Reverse(TRng& Rng)
    {
        //TODO: own view & view in support
        return TReverseView<TRefView<TRng>>({}, TRefView<TRng>({}, Rng));
    }
}
