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
#include "UnrealRanges/View/AlgoMixin/Count.h"
#include "UnrealRanges/View/AlgoMixin/Fold.h"
#include "UnrealRanges/View/AlgoMixin/Sum.h"
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<typename TView, typename TFn>
    class TFilterView
        : public FView
        , public TTransformMixin<TFilterView<TView, TFn>>
        , public TFilterMixin<TFilterView<TView, TFn>>
        , public TEnumerateMixin<TFilterView<TView, TFn>>
        , public TTakeMixin<TFilterView<TView, TFn>>
        , public TTakeWhileMixin<TFilterView<TView, TFn>>
        , public TConcatMixin<TFilterView<TView, TFn>>
        , public TToMixin<TFilterView<TView, TFn>>
        , public TMinMaxMixin<TFilterView<TView, TFn>>
        , public TFindFirstMixin<TFilterView<TView, TFn>>
        , public TConditionalInheritance<TView::IsBidir, TFindLastMixin<TFilterView<TView, TFn>>>
        , public TCountMixin<TFilterView<TView, TFn>>
        , public TFoldLeftMixin<TFilterView<TView, TFn>>
        , public TConditionalInheritance<TView::IsBidir, TFoldRightMixin<TFilterView<TView, TFn>>>
        , public TSumMixin<TFilterView<TView, TFn>>
        , public TIteratorMixin<TFilterView<TView, TFn>>
        , public TConditionalInheritance<TView::IsBidir, TReverseIteratorMixin<TFilterView<TView, TFn>>>
        , public TConditionalInheritance<TView::IsBidir, TReverseMixin<TFilterView<TView, TFn>>>
    {
        friend struct FCursorProtocol;

    public:
        using reference = typename TView::reference;
        using const_reference = typename TView::const_reference;
        using value_type = typename TView::value_type;
        using Cursor = typename TView::Cursor;
        using ConstCursor = typename TView::ConstCursor;
        using ReverseCursor = typename TView::ReverseCursor;
        using ReverseConstCursor = typename TView::ReverseConstCursor;

        static constexpr bool IsBidir = TView::IsBidir;
        static constexpr bool IsSized = false;

        template<typename UView, typename UFn>
        TFilterView(UView InView, UFn InFn)
            : View(InView)
            , Fn(InFn)
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Misc::ELoop InternalIteration(TSelf& Self, TCallback Callback)
        {
            return FCursorProtocol::InternalIteration<IsForward>(Self.View, [&](auto&& Item)
                {
                    if (std::invoke(Self.Fn, Item))
                    {
                        return Callback(UR_FWD(Item));
                    }
                    return Misc::ELoop::Continue;
                });
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            auto Curs = FCursorProtocol::CursorBegin<IsForward>(Self.View);
            FastForward(Self, Curs);

            return Curs;
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return FCursorProtocol::CursorEnd<IsForward>(Self.View);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            FCursorProtocol::CursorInc(Self.View, Curs);
            FastForward(Self, Curs);
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
        template<typename TSelf, typename TCursor>
        static void FastForward(TSelf& Self, TCursor& Curs)
        {
            while (!FCursorProtocol::IsEnd(Self.View, Curs) && !std::invoke(Self.Fn, FCursorProtocol::CursorDeref(Self.View, Curs)))
            {
                FCursorProtocol::CursorInc(Self.View, Curs);
            }
        }

    private:
        TView View;
        TFn Fn;
    };


    template<typename TRng, typename TFn>
    auto Filter(TRng& Rng, TFn Fn)
    {
        return TFilterView<TRefView<TRng>, TFn>(TRefView<TRng>({}, Rng), Fn);
    }
}