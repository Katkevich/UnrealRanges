#pragma once
#include "UnrealRanges/Detail/AlwaysFalse.h"
#include "UnrealRanges/Traits.h"
#include <type_traits>

namespace Ur::View {

    template<Ur::RangeView... TViews>
    class TConcatView;

    template<Ur::Range TRng>
    class TRefView;

    template<typename TView>
    struct TConcatMixin
    {
        template<Ur::Range... TRngs>
        auto ConcatWith(TRngs&&... Rngs) const&
        {
            return ConcatAll(static_cast<const TView&>(*this), this->IntoView(UR_FWD(Rngs))...);
        }

        template<Ur::Range... TRngs>
        auto ConcatWith(TRngs&&... Rngs) &&
        {
            return ConcatAll(static_cast<TView&&>(*this), this->IntoView(UR_FWD(Rngs))...);
        }

    private:
        template<typename... TViews>
        static auto ConcatAll(TViews&&... Views)
        {
            return TConcatView<std::remove_cvref_t<TViews>...>({}, UR_FWD(Views)...);
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