#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<Ur::RangeView TUnderlView, std::integral TAmount = int32>
    class TDropView
        : public FView
        , public Detail::TMixins<TDropView<TUnderlView, TAmount>, TDefaultMixins>
        , public Detail::TConditionalMixins<TUnderlView::LikeMap, TDropView<TUnderlView, TAmount>, TMapMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = typename TUnderlView::reference;
        using const_reference = typename TUnderlView::const_reference;
        using value_type = typename TUnderlView::value_type;

        using Cursor = typename TUnderlView::Cursor;
        using ConstCursor = typename TUnderlView::ConstCursor;
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = TUnderlView::IsSized;
        static constexpr bool LikeMap = TUnderlView::LikeMap;

        template<typename UUnderlView, typename UAmount>
        TDropView(UUnderlView&& InView, UAmount InAmount)
            : View(UR_FWD(InView))
            , Amount(InAmount)
        {
            ensure(Amount >= 0);
        }

        auto Num() const requires IsSized
        {
            auto ViewSize = Ur::Size(View);
            using NumType = decltype(ViewSize);

            return (ViewSize < NumType(Amount))
                ? NumType(0)
                : NumType(ViewSize - Amount);
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            TAmount Counter = { 0 };
            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
                {
                    if (Counter < Self.Amount)
                        ++Counter;
                    else
                        return Callback(UR_FWD(Item));

                    return Ur::ELoop::Continue;
                });
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            auto Curs = Ur::Cursor::Begin<IsForward>(Self.View);

            for (auto Idx = 0; Idx < Self.Amount && !Ur::Cursor::IsEnd(Self.View, Curs); ++Idx)
                Ur::Cursor::Inc(Self.View, Curs);

            return Curs;
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
            return Ur::Cursor::Deref(Self.View, Curs);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return Ur::Cursor::Eq(Self.View, Lhs, Rhs);
        }

    private:
        TUnderlView View;
        TAmount Amount;
    };


    template<typename TRng, typename TAmount = int32>
    auto Drop(TRng& Rng, TAmount Amount)
    {
        //TODO: own view & view in support
        return TDropView<TRefView<TRng>, TAmount>(TRefView<TRng>({}, Rng), Amount);
    }
}