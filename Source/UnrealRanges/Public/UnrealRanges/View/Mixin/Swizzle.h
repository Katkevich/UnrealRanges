#pragma once
#include "UnrealRanges/Traits.h"
#include <cstddef>

namespace Ur::View {
    namespace Detail {
        template<std::size_t N, std::size_t... Is>
        consteval bool CheckDuplicates()
        {
            // make sure that each index is from range [0..N-1] and each index is unique
            bool bHasAllIndices = true;
            for (std::size_t I = 0; I < N; ++I)
                bHasAllIndices &= ((Is == I) || ...);

            return bHasAllIndices;
        }

        // std::tuple_element isn't sfinae friendly so checking only part of a tuple protocol
        template<typename TView, std::size_t... ItemsOrder>
        concept SwizzleIndices =
            Ur::TTupleSize_V<typename TView::value_type> == sizeof...(ItemsOrder) &&
            Ur::View::Detail::CheckDuplicates<sizeof...(ItemsOrder), ItemsOrder...>();
    }

    template<Ur::RangeView TUnderlView, std::size_t... ItemsOrder>
    class TSwizzleView;

    template<typename TView>
    struct TSwizzleMixin
    {
        template<std::size_t... ItemsOrder>
        auto Swizzle() const&
        {
            static_assert(Detail::SwizzleIndices<TView, ItemsOrder...>, "swizzle indices should be unique and be in the range of [0..N-1]");
            return TSwizzleView<TView, ItemsOrder...>({}, static_cast<const TView&>(*this));
        }

        template<std::size_t... ItemsOrder>
        auto Swizzle() &&
        {
            static_assert(Detail::SwizzleIndices<TView, ItemsOrder...>, "swizzle indices should be unique and be in the range of [0..N-1]");
            return TSwizzleView<TView, ItemsOrder...>({}, static_cast<TView&&>(*this));
        }
    };
}