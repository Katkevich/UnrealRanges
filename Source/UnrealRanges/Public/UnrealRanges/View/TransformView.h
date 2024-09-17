#pragma once
#include "UnrealRanges/View/Mixin/Reverse.h"
#include "UnrealRanges/View/Mixin/Transform.h"
#include "UnrealRanges/View/Mixin/Filter.h"
#include "UnrealRanges/View/Mixin/Enumerate.h"
#include "UnrealRanges/View/Mixin/Take.h"
#include "UnrealRanges/View/Mixin/TakeWhile.h"
#include "UnrealRanges/View/Mixin/Concat.h"
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
    template<typename TView, typename TFn>
    class TTransformView
        : public FView
        , public TTransformMixin<TTransformView<TView, TFn>>
        , public TFilterMixin<TTransformView<TView, TFn>>
        , public TEnumerateMixin<TTransformView<TView, TFn>>
        , public TTakeMixin<TTransformView<TView, TFn>>
        , public TTakeWhileMixin<TTransformView<TView, TFn>>
        , public TConcatMixin<TTransformView<TView, TFn>>
        , public TToMixin<TTransformView<TView, TFn>>
        , public TMinMaxMixin<TTransformView<TView, TFn>>
        , public TFindFirstMixin<TTransformView<TView, TFn>>
        , public TConditionalInheritance<TView::IsBidir, TFindLastMixin<TTransformView<TView, TFn>>>
        , public TCountMixin<TTransformView<TView, TFn>>
        , public TFoldLeftMixin<TTransformView<TView, TFn>>
        , public TConditionalInheritance<TView::IsBidir, TFoldRightMixin<TTransformView<TView, TFn>>>
        , public TConditionalInheritance<TView::IsSized, TSizeMixin<TTransformView<TView, TFn>>>
        , public TSumMixin<TTransformView<TView, TFn>>
        , public TIteratorMixin<TTransformView<TView, TFn>>
        , public TConditionalInheritance<TView::IsBidir, TReverseIteratorMixin<TTransformView<TView, TFn>>>
        , public TConditionalInheritance<TView::IsBidir, TReverseMixin<TTransformView<TView, TFn>>>
    {
        friend struct FCursorProtocol;

    public:
        using reference = std::invoke_result_t<TFn, typename TView::reference>;
        using value_type = std::remove_cvref_t<reference>;
        using const_reference = std::conditional_t<
            std::is_lvalue_reference_v<reference>,
            std::add_lvalue_reference_t<std::add_const_t<value_type>>,
            value_type
        >;

        using Cursor = typename TView::Cursor;
        using ReverseCursor = typename TView::ReverseCursor;

        static constexpr bool IsBidir = TView::IsBidir;
        static constexpr bool IsSized = TView::IsSized;

        template<typename UView, typename UFn>
        TTransformView(UView InView, UFn InFn)
            : View(InView)
            , Fn(InFn)
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Misc::ELoop InternalIteration(TSelf& Self, TCallback Callback)
        {
            return FCursorProtocol::InternalIteration(Misc::Same<IsForward>, Self.View, [&](auto&& Item)
                {
                    return Callback(std::invoke(Self.Fn, UR_FWD(Item)));
                });
        }

        UR_DEBUG_NOINLINE Cursor CursorBegin() const
        {
            return FCursorProtocol::CursorBegin(View);
        }

        UR_DEBUG_NOINLINE Cursor CursorEnd() const
        {
            return FCursorProtocol::CursorEnd(View);
        }

        UR_DEBUG_NOINLINE ReverseCursor CursorRBegin() const requires IsBidir
        {
            return FCursorProtocol::CursorRBegin(View);
        }

        UR_DEBUG_NOINLINE ReverseCursor CursorREnd() const requires IsBidir
        {
            return FCursorProtocol::CursorREnd(View);
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE void CursorInc(TCursor& Curs) const
        {
            return FCursorProtocol::CursorInc(View, Curs);
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE reference CursorDeref(const TCursor& Curs) const
        {
            return std::invoke(Fn, FCursorProtocol::CursorDeref(View, Curs));
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE bool CursorEq(const TCursor& Lhs, const TCursor& Rhs) const
        {
            return FCursorProtocol::CursorEq(View, Lhs, Rhs);
        }

    private:
        TView View;
        TFn Fn;
    };


    template<typename TRng, typename TFn>
    auto Transform(TRng& Rng, TFn Fn)
    {
        return TTransformView<TRefView<TRng>, TFn>(TRefView<TRng>({}, Rng), Fn);
    }
}