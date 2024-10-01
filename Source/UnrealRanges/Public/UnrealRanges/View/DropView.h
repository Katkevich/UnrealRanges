#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<typename TView, typename TAmount>
    class TDropView
        : public FView
        , public Detail::TMixins<TDropView<TView, TAmount>, TDefaultMixins>
        , public Detail::TConditionalMixins<TView::LikeMap, TDropView<TView, TAmount>, TMapMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = typename TView::reference;
        using const_reference = typename TView::const_reference;
        using value_type = typename TView::value_type;

        using Cursor = typename TView::Cursor;
        using ConstCursor = typename TView::ConstCursor;
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = TView::IsSized;
        static constexpr bool LikeMap = TView::LikeMap;

        template<typename UView, typename UAmount>
        TDropView(UView&& InView, UAmount InAmount)
            : View(UR_FWD(InView))
            , Amount(InAmount)
        {
        }

        auto Num() const requires IsSized
        {
            auto ViewSize = Ur::Size(View);
            return (ViewSize < Amount) ? 0 : (ViewSize - Amount);
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Misc::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            TAmount Counter = { 0 };
            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
                {
                    if (Counter < Self.Amount)
                        ++Counter;
                    else
                        return Callback(UR_FWD(Item));

                    return Misc::ELoop::Continue;
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
        TView View;
        TAmount Amount;
    };


    template<typename TRng, typename TAmount = int32>
    auto Drop(TRng& Rng, TAmount Amount)
    {
        //TODO: own view & view in support
        return TDropView<TRefView<TRng>, TAmount>(TRefView<TRng>({}, Rng), Amount);
    }
}