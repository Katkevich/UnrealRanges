#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<typename TView, typename TFn>
    class TFilterView
        : public FView
        , public Detail::TMixins<TFilterView<TView, TFn>, TDefaultMixins>
        , public Detail::TConditionalMixins<TView::IsBidir, TFilterView<TView, TFn>, TBidirMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = typename TView::reference;
        using const_reference = typename TView::const_reference;
        using value_type = typename TView::value_type;
        using Cursor = typename TView::Cursor;
        using ConstCursor = typename TView::ConstCursor;
        using ReverseCursor = typename TView::ReverseCursor;
        using ReverseConstCursor = typename TView::ReverseConstCursor;

        static constexpr bool IsBidir = TView::IsBidir;
        static constexpr bool IsSized = false;

        template<typename UView, typename UFn>
        TFilterView(UView&& InView, UFn&& InFn)
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
                    return Misc::ELoop::Continue;
                });
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            auto Curs = Ur::Cursor::Begin<IsForward>(Self.View);
            FastForward(Self, Curs);

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
            Ur::Cursor::Inc(Self.View, Curs);
            FastForward(Self, Curs);
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
        template<typename TSelf, typename TCursor>
        static void FastForward(TSelf& Self, TCursor& Curs)
        {
            while (!Ur::Cursor::IsEnd(Self.View, Curs) && !std::invoke(Self.Fn, Ur::Cursor::Deref(Self.View, Curs)))
            {
                Ur::Cursor::Inc(Self.View, Curs);
            }
        }

    private:
        TView View;
        TFn Fn;
    };


    template<typename TRng, typename TFn>
    auto Filter(TRng& Rng, TFn&& Fn)
    {
        //TODO: own view & view in support
        return TFilterView<TRefView<TRng>, std::decay_t<TFn>>(TRefView<TRng>({}, Rng), UR_FWD(Fn));
    }
}