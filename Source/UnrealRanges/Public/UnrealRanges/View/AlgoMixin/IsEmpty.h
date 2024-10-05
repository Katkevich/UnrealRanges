#pragma once
#include "UnrealRanges/Traits.h"

namespace Ur::View {
    template<typename TView>
    struct TIsEmptyMixin
    {
        bool IsEmpty() const
        {
            if constexpr (SizedRange<TView>)
            {
                return Ur::Size(*static_cast<const TView*>(this)) == 0;
            }
            else
            {
                auto Self = static_cast<const TView*>(this);
                return Self->begin() == Self->end();
            }
        }
    };
}