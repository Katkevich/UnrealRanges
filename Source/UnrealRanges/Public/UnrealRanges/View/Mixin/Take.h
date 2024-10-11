#pragma once
#include "UnrealRanges/Traits.h"

namespace Ur::View {

    template<Ur::RangeView TView, std::integral TAmount>
    class TTakeView;

    template<typename TView>
    struct TTakeMixin
    {
        template<std::integral TAmount>
        auto Take(TAmount Amount) const&
        {
            return TTakeView<TView, TAmount>(static_cast<const TView&>(*this), Amount);
        }

        template<std::integral TAmount>
        auto Take(TAmount Amount) &&
        {
            return TTakeView<TView, TAmount>(static_cast<TView&&>(*this), Amount);
        }
    };
}