#pragma once
#include "UnrealRanges/View/Mixin/Transform.h"
#include "UnrealRanges/View/Mixin/Reverse.h"
#include "UnrealRanges/View/Mixin/Iterator.h"
#include "UnrealRanges/View/AlgoMixin/To.h"
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"

namespace Ur::View {

    template<typename TView>
    class TReverseView
        : public FView
        , public TReverseMixin<TReverseView<TView>>
        , public TIteratorMixin<TReverseView<TView>>
        , public TReverseIteratorMixin<TReverseView<TView>>
        , public TToMixin<TReverseView<TView>>
    {
        friend struct FCursorProtocol;

    public:
        using value_type = typename TView::value_type;
        using reference = typename TView::reference;
        using const_reference = typename TView::const_reference;

        using Cursor = typename TView::ReverseCursor;
        using ReverseCursor = typename TView::Cursor;

        static constexpr bool IsBiDir = true;

        template<typename UView>
        TReverseView(Misc::FFromViewTag, UView InView)
            : View(InView)
        {
        }

    private:
        template<bool IsForward, typename TFn>
        void InternalIteration(TFn Fn)
        {
            FCursorProtocol::InternalIteration(Direction::Opposite<IsForward>, View, Fn);
        }

        Cursor CursorBegin() const
        {
            return FCursorProtocol::CursorRBegin(View);
        }

        Cursor CursorEnd() const
        {
            return FCursorProtocol::CursorREnd(View);
        }

        ReverseCursor CursorRBegin() const
        {
            return FCursorProtocol::CursorBegin(View);
        }

        ReverseCursor CursorREnd() const
        {
            return FCursorProtocol::CursorEnd(View);
        }

        template<typename TCursor>
        void CursorInc(TCursor& Curs) const
        {
            ++Curs;
        }

        template<typename TCursor>
        reference CursorDeref(const TCursor& Curs) const
        {
            return *Curs;
        }

        template<typename TCursor>
        bool CursorEq(const TCursor& Lhs, const TCursor& Rhs) const
        {
            return !(Lhs != Rhs);
        }

    private:
        TView View;
    };


    template<typename TRng>
    auto Reverse(TRng& Rng)
    {
        return TReverseView<TRefView<TRng>>({}, TRefView<TRng>({}, Rng));
    }
}
