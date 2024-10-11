#pragma once
#include "UnrealRanges/Traits.h"

namespace Ur::View {

    template<Ur::RangeView TView>
    class TValuesView;

    template<typename TView>
    struct TValuesMixin
    {
        auto Values() const&
        {
            return TValuesView<TView>({}, static_cast<const TView&>(*this));
        }

        auto Values()&&
        {
            return TValuesView<TView>({}, static_cast<TView&&>(*this));
        }
    };
}