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
    template<typename TView, typename TAmount>
    class TTakeView
        : public FView
        , public TTransformMixin<TTakeView<TView, TAmount>>
        , public TFilterMixin<TTakeView<TView, TAmount>>
        , public TEnumerateMixin<TTakeView<TView, TAmount>>
        , public TTakeMixin<TTakeView<TView, TAmount>>
        , public TTakeWhileMixin<TTakeView<TView, TAmount>>
        , public TConcatMixin<TTakeView<TView, TAmount>>
        , public TToMixin<TTakeView<TView, TAmount>>
        , public TMinMaxMixin<TTakeView<TView, TAmount>>
        , public TFindFirstMixin<TTakeView<TView, TAmount>>
        , public TCountMixin<TTakeView<TView, TAmount>>
        , public TFoldLeftMixin<TTakeView<TView, TAmount>>
        , public TSumMixin<TTakeView<TView, TAmount>>
        , public TIteratorMixin<TTakeView<TView, TAmount>>
    {
        friend struct FCursorProtocol;

    public:
        using reference = typename TView::reference;
        using const_reference = typename TView::const_reference;
        using value_type = typename TView::value_type;

        struct Cursor
        {
            typename TView::Cursor Nested{};
            TAmount Index{};
        };
        using ReverseCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = TView::IsSized;

        template<typename UView, typename UAmount>
        TTakeView(UView InView, UAmount InAmount)
            : View(InView)
            , Amount(InAmount)
        {
        }

        auto Num() const requires IsSized
        {
            // we don't want to have dependency on <algorithm> coz it is too heavy to bring in for such simple algorithm
            return [](auto&& Lhs, auto&& Rhs) {
                return (Lhs < Rhs) ? Lhs : Rhs;
            }(Ur::Size(View), Amount);
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Misc::ELoop InternalIteration(TSelf& Self, TCallback Callback)
        {
            TAmount Counter = { 0 };
            return FCursorProtocol::InternalIteration(Misc::Same<IsForward>, Self.View, [&](auto&& Item)
                {
                    if (Counter < Self.Amount)
                    {
                        ++Counter;
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
            return Cursor{ FCursorProtocol::CursorBegin(View), TAmount{ 0 } };
        }

        UR_DEBUG_NOINLINE Cursor CursorEnd() const
        {
            return Cursor{ FCursorProtocol::CursorEnd(View), Amount };
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE void CursorInc(TCursor& Curs) const
        {
            ++Curs.Index;
            return FCursorProtocol::CursorInc(View, Curs.Nested);
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE reference CursorDeref(const TCursor& Curs) const
        {
            return FCursorProtocol::CursorDeref(View, Curs.Nested);
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE bool CursorEq(const TCursor& Lhs, const TCursor& Rhs) const
        {
            return
                // either all equal
                (Lhs.Index == Rhs.Index && FCursorProtocol::CursorEq(View, Lhs.Nested, Rhs.Nested)) ||
                // or both are End cursor (either Index reached Amount or nested cursor reached End)
                (
                    (Lhs.Index == Amount || FCursorProtocol::IsEnd(Misc::Forward, View, Lhs.Nested)) &&
                    (Rhs.Index == Amount || FCursorProtocol::IsEnd(Misc::Forward, View, Rhs.Nested))
                );
        }

    private:
        TView View;
        TAmount Amount;
    };


    template<typename TRng, typename TAmount = int32>
    auto Take(TRng& Rng, TAmount Amount)
    {
        return TTakeView<TRefView<TRng>, TAmount>(TRefView<TRng>({}, Rng), Amount);
    }
}