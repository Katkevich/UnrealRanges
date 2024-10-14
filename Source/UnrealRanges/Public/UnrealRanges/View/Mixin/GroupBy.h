#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "UnrealRanges/Traits.h"
#include "Templates/IdentityFunctor.h"
#include <type_traits>

namespace Ur::View {

    template<Ur::RangeView TUnderlView, std::regular_invocable<typename TUnderlView::reference> TFn>
    class TGroupByView;

    template<typename TView>
    struct TGroupByMixin
    {
        template<typename TFn = FIdentityFunctor>
        auto GroupBy(TFn&& Fn = {}) const&
        {
            return TGroupByView<TView, std::decay_t<TFn>>(static_cast<const TView&>(*this), UR_FWD(Fn));
        }

        template<typename TFn = FIdentityFunctor>
        auto GroupBy(TFn&& Fn = {}) &&
        {
            return TGroupByView<TView, std::decay_t<TFn>>(static_cast<TView&&>(*this), UR_FWD(Fn));
        }
    };
}