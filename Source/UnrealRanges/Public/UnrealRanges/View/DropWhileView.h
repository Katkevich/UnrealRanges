#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<typename TView, typename TFn>
    class TDropWhileView
        : public FView
        , public Detail::TMixins<TDropWhileView<TView, TFn>, TDefaultMixins>
        , public Detail::TConditionalMixins<TView::LikeMap, TDropWhileView<TView, TFn>, TMapMixins>
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
        static constexpr bool IsSized = false;
        static constexpr bool LikeMap = TView::LikeMap;

        template<typename UView, typename UFn>
        TDropWhileView(UView&& InView, UFn&& InFn)
            : View(UR_FWD(InView))
            , Fn(UR_FWD(InFn))
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Misc::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            bool bKeepSkipping = true;

            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
                {
                    if (bKeepSkipping && std::invoke(Self.Fn, Item))
                    {
                        return Misc::ELoop::Continue;
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
        TView View;
        TFn Fn;
    };


    template<typename TRng, typename TFn>
    auto DropWhile(TRng& Rng, TFn&& Fn)
    {
        //TODO: own view & view in support
        return TDropWhileView<TRefView<TRng>, std::decay_t<TFn>>(TRefView<TRng>({}, Rng), UR_FWD(Fn));
    }
}