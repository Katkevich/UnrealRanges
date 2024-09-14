#pragma once
#include "UnrealRanges/View/Mixin/Reverse.h"
#include "UnrealRanges/View/Mixin/Transform.h"
#include "UnrealRanges/View/Mixin/Filter.h"
#include "UnrealRanges/View/Mixin/Enumerate.h"
#include "UnrealRanges/View/Mixin/Take.h"
#include "UnrealRanges/View/Mixin/TakeWhile.h"
#include "UnrealRanges/View/Mixin/Iterator.h"
#include "UnrealRanges/View/AlgoMixin/To.h"
#include "UnrealRanges/View/AlgoMixin/MinMax.h"
#include "UnrealRanges/View/AlgoMixin/Find.h"
#include "UnrealRanges/View/AlgoMixin/FindLast.h"
#include "UnrealRanges/View/AlgoMixin/Count.h"
#include "UnrealRanges/View/AlgoMixin/Fold.h"
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
        , public TToMixin<TFilterView<TView, TFn>>
        , public TMinMaxMixin<TFilterView<TView, TFn>>
        , public TFindMixin<TFilterView<TView, TFn>>
        , public TConditionalInheritance<TView::IsBidir, TFindLastMixin<TFilterView<TView, TFn>>>
        , public TCountMixin<TFilterView<TView, TFn>>
        , public TFoldLeftMixin<TFilterView<TView, TFn>>
        , public TConditionalInheritance<TView::IsBidir, TFoldRightMixin<TFilterView<TView, TFn>>>
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
        using ReverseCursor = typename TView::ReverseCursor;

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
        UR_DEBUG_NOINLINE static void InternalIteration(TSelf& Self, TCallback Callback)
        {
            FCursorProtocol::InternalIteration(Misc::Same<IsForward>, Self.View, [&](auto&& Item)
                {
                    if (std::invoke(Self.Fn, Item))
                    {
                        return Callback(UR_FWD(Item));
                    }
                    return Misc::ELoop::Continue;
                });
        }

        UR_DEBUG_NOINLINE Cursor CursorBegin() const
        {
            auto Curs = FCursorProtocol::CursorBegin(View);
            this->FastForward(Curs);

            return Curs;
        }

        UR_DEBUG_NOINLINE Cursor CursorEnd() const
        {
            return FCursorProtocol::CursorEnd(View);
        }

        UR_DEBUG_NOINLINE ReverseCursor CursorRBegin() const requires IsBidir
        {
            auto Curs = FCursorProtocol::CursorRBegin(View);
            this->FastForward(Curs);

            return Curs;
        }

        UR_DEBUG_NOINLINE ReverseCursor CursorREnd() const requires IsBidir
        {
            return FCursorProtocol::CursorREnd(View);
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE void CursorInc(TCursor& Curs) const
        {
            FCursorProtocol::CursorInc(View, Curs);
            this->FastForward(Curs);
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE reference CursorDeref(const TCursor& Curs) const
        {
            return FCursorProtocol::CursorDeref(View, Curs);
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE bool CursorEq(const TCursor& Lhs, const TCursor& Rhs) const
        {
            return FCursorProtocol::CursorEq(View, Lhs, Rhs);
        }

    private:
        template<typename TCursor>
        void FastForward(TCursor& Curs) const
        {
            const auto CursEnd = this->CursorEndOfType<TCursor>();

            while (!FCursorProtocol::CursorEq(View, Curs, CursEnd) && !std::invoke(Fn, FCursorProtocol::CursorDeref(View, Curs)))
            {
                FCursorProtocol::CursorInc(View, Curs);
            }
        }

        template<typename TCursor>
        auto CursorEndOfType() const
        {
            if constexpr (std::is_same_v<TCursor, Cursor>)
                return this->CursorEnd();
            else
                return this->CursorREnd();
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