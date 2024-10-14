#pragma once
#include "UnrealRanges/Traits.h"
#include <cstddef>

namespace Ur::View {

    template<Ur::RangeView TUnderlView, std::size_t N>
    requires (N > 0)
    class TAdjacentView;

    template<typename TView>
    struct TAdjacentMixin
    {
        template<std::size_t N = 2>
        auto Adjacent() const&
        {
            return TAdjacentView<TView, N>({}, static_cast<const TView&>(*this));
        }

        template<std::size_t N = 2>
        auto Adjacent() &&
        {
            return TAdjacentView<TView, N>({}, static_cast<TView&&>(*this));
        }

        auto Pairwise() const&
        {
            return TAdjacentView<TView, 2>({}, static_cast<const TView&>(*this));
        }

        auto Pairwise()&&
        {
            return TAdjacentView<TView, 2>({}, static_cast<TView&&>(*this));
        }
    };
}