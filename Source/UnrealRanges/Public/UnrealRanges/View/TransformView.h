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
        friend struct Ur::Cursor;

    public:
        using reference = std::invoke_result_t<TFn, typename TView::reference>;
        using value_type = std::remove_cvref_t<reference>;
        using const_reference = std::conditional_t<
            std::is_lvalue_reference_v<reference>,
            std::add_lvalue_reference_t<std::add_const_t<value_type>>,
            value_type
        >;

        using Cursor = typename TView::Cursor;
        using ConstCursor = typename TView::ConstCursor;
        using ReverseCursor = typename TView::ReverseCursor;
        using ReverseConstCursor = typename TView::ReverseConstCursor;

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
        UR_DEBUG_NOINLINE static Misc::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
                {
                    return Callback(std::invoke(Self.Fn, UR_FWD(Item)));
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
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return std::invoke(Self.Fn, Ur::Cursor::Deref(Self.View, Curs));
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return Ur::Cursor::Eq(Self.View, Lhs, Rhs);
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