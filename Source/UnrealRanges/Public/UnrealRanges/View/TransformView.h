#pragma once
#include "UnrealRanges/View/Mixin/Reverse.h"
#include "UnrealRanges/View/Mixin/Iterator.h"
#include "UnrealRanges/View/AlgoMixin/To.h"
#include "UnrealRanges/View/AlgoMixin/MinMax.h"
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {
    template<typename TView, typename TFn>
    class TTransformView
        : public FView
        , public TTransformMixin<TTransformView<TView, TFn>>
        , public TToMixin<TTransformView<TView, TFn>>
        , public TMinMaxMixin<TTransformView<TView, TFn>>
        , public TIteratorMixin<TTransformView<TView, TFn>>
        , public TConditionalInheritance<TView::IsBidir, TReverseIteratorMixin<TTransformView<TView, TFn>>>
        , public TConditionalInheritance<TView::IsBidir, TReverseMixin<TTransformView<TView, TFn>>>
    {
    public:
        using reference = std::invoke_result_t<TFn, typename TView::reference>;
        using value_type = std::remove_reference_t<reference>;
        using Cursor = typename TView::Cursor;
        using ReverseCursor = typename TView::ReverseCursor;

        static constexpr bool IsBidir = TView::IsBidir;

        template<typename UView, typename UFn>
        TTransformView(UView InView, UFn InFn)
            : View(InView)
            , Fn(InFn)
        {
        }

        template<bool IsForward, typename TSelf, typename TCallback>
        static void InternalIteration(TSelf& Self, TCallback Callback)
        {
            FCursorProtocol::InternalIteration(Misc::Same<IsForward>, Self.View, [&](auto&& Item)
                {
                    return Callback(std::invoke(Self.Fn, UR_FWD(Item)));
                });
        }

        Cursor CursorBegin() const
        {
            return FCursorProtocol::CursorBegin(View);
        }

        Cursor CursorEnd() const
        {
            return FCursorProtocol::CursorEnd(View);
        }

        ReverseCursor CursorRBegin() const requires IsBidir
        {
            return FCursorProtocol::CursorRBegin(View);
        }

        ReverseCursor CursorREnd() const requires IsBidir
        {
            return FCursorProtocol::CursorREnd(View);
        }

        template<typename TCursor>
        void CursorInc(TCursor& Curs) const
        {
            return FCursorProtocol::CursorInc(View, Curs);
        }

        template<typename TCursor>
        reference CursorDeref(const TCursor& Curs) const
        {
            return std::invoke(Fn, FCursorProtocol::CursorDeref(View, Curs));
        }

        template<typename TCursor>
        bool CursorEq(const TCursor& Lhs, const TCursor& Rhs) const
        {
            return FCursorProtocol::CursorEq(View, Lhs, Rhs);
        }

    private:
        TView View;
        TFn Fn;
    };


    template<typename TRng, typename TFn>
    auto Transform(TRng& Rng, TFn Fn)
    {
        return TTransformView<TRefView<TRng>, TFn>(TRefView<TRng>({}, Rng), Fn);
    }
}