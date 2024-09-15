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
#include "UnrealRanges/View/AlgoMixin/Count.h"
#include "UnrealRanges/View/AlgoMixin/Fold.h"
#include "UnrealRanges/View/AlgoMixin/Sum.h"
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<typename TView, typename TFn>
    class TTakeWhileView
        : public FView
        , public TTransformMixin<TTakeWhileView<TView, TFn>>
        , public TFilterMixin<TTakeWhileView<TView, TFn>>
        , public TEnumerateMixin<TTakeWhileView<TView, TFn>>
        , public TTakeMixin<TTakeWhileView<TView, TFn>>
        , public TTakeWhileMixin<TTakeWhileView<TView, TFn>>
        , public TToMixin<TTakeWhileView<TView, TFn>>
        , public TMinMaxMixin<TTakeWhileView<TView, TFn>>
        , public TFindFirstMixin<TTakeWhileView<TView, TFn>>
        , public TCountMixin<TTakeWhileView<TView, TFn>>
        , public TFoldLeftMixin<TTakeWhileView<TView, TFn>>
        , public TSumMixin<TTakeWhileView<TView, TFn>>
        , public TIteratorMixin<TTakeWhileView<TView, TFn>>
    {
        friend struct FCursorProtocol;

    public:
        using reference = typename TView::reference;
        using const_reference = typename TView::const_reference;
        using value_type = typename TView::value_type;

        struct Cursor
        {
            typename TView::Cursor Nested;
            bool bIsEnd = false;
        };
        using ReverseCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = false;

        template<typename UView, typename UFn>
        TTakeWhileView(UView InView, UFn InFn)
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
                    else
                    {
                        return Misc::ELoop::Break;
                    }
                });
        }

        UR_DEBUG_NOINLINE Cursor CursorBegin() const
        {
            auto NestedCursor = FCursorProtocol::CursorBegin(View);
            const auto bIsEnd = FCursorProtocol::IsEnd(View, NestedCursor) || !std::invoke(Fn, FCursorProtocol::CursorDeref(View, NestedCursor));

            return Cursor{ MoveTemp(NestedCursor), bIsEnd };
        }

        UR_DEBUG_NOINLINE Cursor CursorEnd() const
        {
            return Cursor{ FCursorProtocol::CursorEnd(View), true };
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE void CursorInc(TCursor& Curs) const
        {
            FCursorProtocol::CursorInc(View, Curs.Nested);

            Curs.bIsEnd = FCursorProtocol::IsEnd(View, Curs.Nested) || !std::invoke(Fn, FCursorProtocol::CursorDeref(View, Curs.Nested));
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE reference CursorDeref(const TCursor& Curs) const
        {
            return FCursorProtocol::CursorDeref(View, Curs.Nested);
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE bool CursorEq(const TCursor& Lhs, const TCursor& Rhs) const
        {
            return Lhs.bIsEnd == Rhs.bIsEnd && (Lhs.bIsEnd || FCursorProtocol::CursorEq(View, Lhs.Nested, Rhs.Nested));
        }

    private:
        TView View;
        TFn Fn;
    };


    template<typename TRng, typename TFn = FIdentityFunctor>
    auto TakeWhile(TRng& Rng, TFn Fn = {})
    {
        return TTakeWhileView<TRefView<TRng>, TFn>(TRefView<TRng>({}, Rng), Fn);
    }
}