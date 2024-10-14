#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<Ur::RangeView TUnderlView, ViewPredicate<TUnderlView> TFn>
    class TDropWhileView
        : public FView
        , public Detail::TMixins<TDropWhileView<TUnderlView, TFn>, TDefaultMixins>
        , public Detail::TConditionalMixins<TUnderlView::LikeMap, TDropWhileView<TUnderlView, TFn>, TMapMixins>
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
        static constexpr bool IsSized = false;
        static constexpr bool LikeMap = TUnderlView::LikeMap;

        template<typename UUnderlView, typename UFn>
        TDropWhileView(UUnderlView&& InView, UFn&& InFn)
            : View(UR_FWD(InView))
            , Fn(UR_FWD(InFn))
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            bool bKeepSkipping = true;

            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
                {
                    if (bKeepSkipping && std::invoke(Self.Fn, Item))
                    {
                        return Ur::ELoop::Continue;
                    }
                    else
                    {
                        bKeepSkipping = false;
                        return Callback(UR_FWD(Item));
                    }
                });
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            auto Curs = Ur::Cursor::Begin<IsForward>(Self.View);

            while (!Ur::Cursor::IsEnd(Self.View, Curs) && std::invoke(Self.Fn, Ur::Cursor::Deref(Self.View, Curs)))
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
        TFn Fn;
    };


    template<typename TRng, typename TFn>
    auto DropWhile(TRng& Rng, TFn&& Fn)
    {
        //TODO: own view & view in support
        return TDropWhileView<TRefView<TRng>, std::decay_t<TFn>>(TRefView<TRng>({}, Rng), UR_FWD(Fn));
    }
}