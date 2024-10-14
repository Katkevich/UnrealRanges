#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<Ur::RangeView TUnderlView, std::integral TAmount = int32>
    class TTakeView
        : public FView
        , public Detail::TMixins<TTakeView<TUnderlView, TAmount>, TDefaultMixins>
        , public Detail::TConditionalMixins<TUnderlView::LikeMap, TTakeView<TUnderlView, TAmount>, TMapMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = typename TUnderlView::reference;
        using const_reference = typename TUnderlView::const_reference;
        using value_type = typename TUnderlView::value_type;

        struct Cursor
        {
            typename TUnderlView::Cursor Nested{};
            TAmount Index{};
        };
        struct ConstCursor
        {
            typename TUnderlView::ConstCursor Nested{};
            TAmount Index{};
        };
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = TUnderlView::IsSized;
        static constexpr bool LikeMap = TUnderlView::LikeMap;

        template<typename UUnderlView, typename UAmount>
        TTakeView(UUnderlView&& InView, UAmount InAmount)
            : View(UR_FWD(InView))
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
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            TAmount Counter = { 0 };
            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
                {
                    if (Counter < Self.Amount)
                    {
                        ++Counter;
                        return Callback(UR_FWD(Item));
                    }
                    else
                    {
                        return Ur::ELoop::Break;
                    }
                });
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return TCursor<TSelf, IsForward>{ Ur::Cursor::Begin<IsForward>(Self.View), TAmount{ 0 } };
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return TCursor<TSelf, IsForward>{ Ur::Cursor::End<IsForward>(Self.View), Self.Amount };
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            ++Curs.Index;
            return Ur::Cursor::Inc(Self.View, Curs.Nested);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return Ur::Cursor::Deref(Self.View, Curs.Nested);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return
                // either all equal
                (Lhs.Index == Rhs.Index && Ur::Cursor::Eq(Self.View, Lhs.Nested, Rhs.Nested)) ||
                // or both are End cursor (either Index reached Amount or nested cursor reached End)
                (
                    (Lhs.Index == Self.Amount || Ur::Cursor::IsEnd(Self.View, Lhs.Nested)) &&
                    (Rhs.Index == Self.Amount || Ur::Cursor::IsEnd(Self.View, Rhs.Nested))
                );
        }

    private:
        TUnderlView View;
        TAmount Amount;
    };


    template<typename TRng, typename TAmount = int32>
    auto Take(TRng& Rng, TAmount Amount)
    {
        //TODO: own view & view in support
        return TTakeView<TRefView<TRng>, TAmount>(TRefView<TRng>({}, Rng), Amount);
    }
}