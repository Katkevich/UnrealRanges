#pragma once
#include "UnrealRanges/View/Mixin/Transform.h"
#include "UnrealRanges/View/Mixin/Filter.h"
#include "UnrealRanges/View/Mixin/Enumerate.h"
#include "UnrealRanges/View/Mixin/Take.h"
#include "UnrealRanges/View/Mixin/TakeWhile.h"
#include "UnrealRanges/View/Mixin/Concat.h"
#include "UnrealRanges/View/Mixin/Reverse.h"
#include "UnrealRanges/View/Mixin/Iterator.h"
#include "UnrealRanges/View/AlgoMixin/To.h"
#include "UnrealRanges/View/AlgoMixin/MinMax.h"
#include "UnrealRanges/View/AlgoMixin/Find.h"
#include "UnrealRanges/View/AlgoMixin/Size.h"
#include "UnrealRanges/View/AlgoMixin/Count.h"
#include "UnrealRanges/View/AlgoMixin/Fold.h"
#include "UnrealRanges/View/AlgoMixin/Sum.h"
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {

    template<typename TView>
    class TReverseView
        : public FView
        , public TTransformMixin<TReverseView<TView>>
        , public TFilterMixin<TReverseView<TView>>
        , public TEnumerateMixin<TReverseView<TView>>
        , public TTakeMixin<TReverseView<TView>>
        , public TTakeWhileMixin<TReverseView<TView>>
        , public TConcatMixin<TReverseView<TView>>
        , public TToMixin<TReverseView<TView>>
        , public TMinMaxMixin<TReverseView<TView>>
        , public TFindFirstMixin<TReverseView<TView>>
        , public TFindLastMixin<TReverseView<TView>>
        , public TCountMixin<TReverseView<TView>>
        , public TFoldLeftMixin<TReverseView<TView>>
        , public TFoldRightMixin<TReverseView<TView>>
        , public TSumMixin<TReverseView<TView>>
        , public TConditionalInheritance<TView::IsSized, TSizeMixin<TReverseView<TView>>>
        , public TIteratorMixin<TReverseView<TView>>
        , public TReverseIteratorMixin<TReverseView<TView>>
        , public TReverseMixin<TReverseView<TView>>
    {
        friend struct FCursorProtocol;

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
        TReverseView(Misc::FFromViewTag, UView InView)
            : View(InView)
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TFn>
        UR_DEBUG_NOINLINE static Misc::ELoop InternalIteration(TSelf& Self, TFn Fn)
        {
            return FCursorProtocol::InternalIteration<Misc::Opposite<IsForward>>(Self.View, Fn);
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return FCursorProtocol::CursorBegin<Misc::Opposite<IsForward>>(Self.View);
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return FCursorProtocol::CursorEnd<Misc::Opposite<IsForward>>(Self.View);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            return FCursorProtocol::CursorInc(Self.View, Curs);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return FCursorProtocol::CursorDeref(Self.View, Curs);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return FCursorProtocol::CursorEq(Self.View, Lhs, Rhs);
        }

    private:
        TView View;
    };


    template<typename TRng>
    auto Reverse(TRng& Rng)
    {
        return TReverseView<TRefView<TRng>>({}, TRefView<TRng>({}, Rng));
    }
}
