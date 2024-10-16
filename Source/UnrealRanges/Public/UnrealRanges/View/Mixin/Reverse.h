#pragma once
#include "UnrealRanges/Traits.h"

namespace Ur::View {

    template<Ur::RangeView TUnderlView>
    class TReverseView;

    template<typename TView>
    struct TReverseMixin
    {
        auto Reverse() const&
        {
            return TReverseView<TView>({}, static_cast<const TView&>(*this));
        }

        auto Reverse() &&
        {
            return TReverseView<TView>({}, static_cast<TView&&>(*this));
        }
    };
}