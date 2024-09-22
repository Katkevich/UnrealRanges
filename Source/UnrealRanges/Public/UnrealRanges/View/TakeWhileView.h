#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<typename TView, typename TFn>
    class TTakeWhileView
        : public FView
        , public Detail::TMixins<TTakeWhileView<TView, TFn>, TDefaultMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = typename TView::reference;
        using const_reference = typename TView::const_reference;
        using value_type = typename TView::value_type;

        struct Cursor
        {
            typename TView::Cursor Nested;
            bool bIsEnd = false;
        };
        struct ConstCursor
        {
            typename TView::ConstCursor Nested;
            bool bIsEnd = false;
        };
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = false;

        template<typename UView, typename UFn>
        TTakeWhileView(UView&& InView, UFn&& InFn)
            : View(UR_FWD(InView))
            , Fn(UR_FWD(InFn))
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Misc::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
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

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            auto NestedCursor = Ur::Cursor::Begin<IsForward>(Self.View);
            const auto bIsEnd = Ur::Cursor::IsEnd(Self.View, NestedCursor) || !std::invoke(Self.Fn, Ur::Cursor::Deref(Self.View, NestedCursor));

            return TCursor<TSelf, IsForward>{ MoveTemp(NestedCursor), bIsEnd };
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return TCursor<TSelf, IsForward>{ Ur::Cursor::End<IsForward>(Self.View), true };
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            Ur::Cursor::Inc(Self.View, Curs.Nested);

            Curs.bIsEnd = Ur::Cursor::IsEnd(Self.View, Curs.Nested) || !std::invoke(Self.Fn, Ur::Cursor::Deref(Self.View, Curs.Nested));
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return Ur::Cursor::Deref(Self.View, Curs.Nested);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return Lhs.bIsEnd == Rhs.bIsEnd && (Lhs.bIsEnd || Ur::Cursor::Eq(Self.View, Lhs.Nested, Rhs.Nested));
        }

    private:
        TView View;
        TFn Fn;
    };


    template<typename TRng, typename TFn = FIdentityFunctor>
    auto TakeWhile(TRng& Rng, TFn&& Fn = {})
    {
        //TODO: own view & view in support
        return TTakeWhileView<TRefView<TRng>, std::decay_t<TFn>>(TRefView<TRng>({}, Rng), UR_FWD(Fn));
    }
}