#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<typename TView, typename TFn>
    class TTransformView
        : public FView
        , public Detail::TMixins<TTransformView<TView, TFn>, TDefaultMixins>
        , public Detail::TConditionalMixins<TView::IsBidir, TTransformView<TView, TFn>, TBidirMixins>
        , public Detail::TConditionalMixins<TView::IsSized, TTransformView<TView, TFn>, TSizedMixins>
        , public Detail::TConditionalMixins<TIsPair_V<std::invoke_result_t<TFn, typename TView::reference>>, TTransformView<TView, TFn>, TMapMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = std::invoke_result_t<TFn, typename TView::reference>;
        using value_type = std::remove_cvref_t<reference>;
        using const_reference = std::conditional_t<
            std::is_lvalue_reference_v<reference>,
            std::add_lvalue_reference_t<std::add_const_t<value_type>>,
            value_type
        >;

        using Cursor = typename TView::Cursor;
        using ConstCursor = typename TView::ConstCursor;
        using ReverseCursor = typename TView::ReverseCursor;
        using ReverseConstCursor = typename TView::ReverseConstCursor;

        static constexpr bool IsBidir = TView::IsBidir;
        static constexpr bool IsSized = TView::IsSized;
        static constexpr bool LikeMap = TIsPair_V<value_type>;

        template<typename UView, typename UFn>
        TTransformView(UView&& InView, UFn&& InFn)
            : View(UR_FWD(InView))
            , Fn(UR_FWD(InFn))
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            return Ur::Cursor::Iterate<IsForward>(Self.View, [&](auto&& Item)
                {
                    return Callback(std::invoke(Self.Fn, UR_FWD(Item)));
                });
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return Ur::Cursor::Begin<IsForward>(Self.View);
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
            return std::invoke(Self.Fn, Ur::Cursor::Deref(Self.View, Curs));
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
    auto Transform(TRng& Rng, TFn&& Fn)
    {
        //TODO: own view & view in support
        return TTransformView<TRefView<TRng>, std::decay_t<TFn>>(TRefView<TRng>({}, Rng), UR_FWD(Fn));
    }
}