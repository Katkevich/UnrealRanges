#pragma once
#include "UnrealRanges/Traits.h"

namespace Ur::View {

    template<Ur::RangeView TView, std::integral TAmount>
    class TDropView;

    template<typename TView>
    struct TDropMixin
    {
        template<std::integral TAmount>
        auto Drop(TAmount Amount) const&
        {
            return TDropView<TView, TAmount>(static_cast<const TView&>(*this), Amount);
        }

        template<std::integral TAmount>
        auto Drop(TAmount Amount) &&
        {
            return TDropView<TView, TAmount>(static_cast<TView&&>(*this), Amount);
        }
    };
}