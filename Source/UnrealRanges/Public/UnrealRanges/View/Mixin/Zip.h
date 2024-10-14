#pragma once
#include "UnrealRanges/Detail/AlwaysFalse.h"
#include "UnrealRanges/Traits.h"
#include <type_traits>

namespace Ur::View {

    template<Ur::RangeView... TUnderlViews>
    class TZipView;

    template<Ur::Range TRng>
    class TRefView;

    template<typename TView>
    struct TZipMixin
    {
        template<Ur::Range... TRngs>
        auto ZipWith(TRngs&&... Rngs) const&
        {
            return ZipAll(static_cast<const TView&>(*this), this->IntoView(UR_FWD(Rngs))...);
        }

        template<Ur::Range... TRngs>
        auto ZipWith(TRngs&&... Rngs)&&
        {
            return ZipAll(static_cast<TView&&>(*this), this->IntoView(UR_FWD(Rngs))...);
        }

    private:
        template<typename... TViews>
        static auto ZipAll(TViews&&... Views)
        {
            return TZipView<std::remove_cvref_t<TViews>...>({}, UR_FWD(Views)...);
        }

        template<typename TRng>
        static decltype(auto) IntoView(TRng&& Rng)
        {
            if constexpr (RangeView<TRng>)
                return UR_FWD(Rng);
            else if constexpr (std::is_lvalue_reference_v<TRng>)
                return TRefView<std::remove_reference_t<TRng>>({}, Rng);
            else
                static_assert(Ur::Detail::TAlwaysFalse<TRng>::Value, "only UR Views and lvalue references are supported; TOwnView not implemented yet");
        }
    };
}