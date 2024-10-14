#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include <limits>

namespace Ur::View {
    template<Ur::RangeView TUnderlView, std::integral TIndex = int32>
    class TEnumerateView
        : public FView
        , public Detail::TMixins<TEnumerateView<TUnderlView, TIndex>, TDefaultMixins>
        , public Detail::TConditionalMixins<TUnderlView::IsSized, TEnumerateView<TUnderlView, TIndex>, TSizedMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = FIndexed<typename TUnderlView::reference, TIndex>;
        using const_reference = FIndexed<typename TUnderlView::const_reference, TIndex>;
        using value_type = reference;

        struct Cursor
        {
            typename TUnderlView::Cursor Nested{};
            TIndex Index{};
        };
        struct ConstCursor
        {
            typename TUnderlView::ConstCursor Nested{};
            TIndex Index{};
        };
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = TUnderlView::IsSized;
        static constexpr bool LikeMap = false;

        template<typename UUnderlView, typename UIndex>
        TEnumerateView(UUnderlView&& InView, UIndex InIndexFrom, UIndex InStep)
            : View(UR_FWD(InView))
            , IndexFrom(InIndexFrom)
            , Step(InStep)
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            TIndex Index = Self.IndexFrom;

            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
                {
                    Ur::ELoop Result = Callback(TReference<TSelf>{ UR_FWD(Item), Index });
                    Index += Self.Step;

                    return Result;
                });
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return TCursor<TSelf, IsForward>{ Ur::Cursor::Begin<IsForward>(Self.View), Self.IndexFrom };
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return TCursor<TSelf, IsForward>{ Ur::Cursor::End<IsForward>(Self.View), std::numeric_limits<TIndex>::max() };
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            Curs.Index += Self.Step;
            return Ur::Cursor::Inc(Self.View, Curs.Nested);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return TReference<TSelf>{ Ur::Cursor::Deref(Self.View, Curs.Nested), Curs.Index };
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return
                Ur::Cursor::Eq(Self.View, Lhs.Nested, Rhs.Nested) &&
                (Lhs.Index == Rhs.Index || Ur::Cursor::IsEnd(Self.View, Lhs.Nested));
        }

    private:
        TUnderlView View;
        TIndex IndexFrom{};
        TIndex Step{};
    };


    template<typename TRng, typename TIndex = int32>
    auto Enumerate(TRng& Rng, TIndex IndexFrom = { 0 }, TIndex Step = TIndex{ 1 })
    {
        //TODO: own view & view in support
        return TEnumerateView<TRefView<TRng>, TIndex>(TRefView<TRng>({}, Rng), IndexFrom, Step);
    }
}