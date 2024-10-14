#pragma once
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "Templates/IdentityFunctor.h"
#include <type_traits>

namespace Ur::View {

    template<Ur::RangeView TUnderlView, ViewPredicate<TUnderlView> TFn>
    class TTakeWhileView;

    template<typename TView>
    struct TTakeWhileMixin
    {
        template<typename TFn = FIdentityFunctor>
        auto TakeWhile(TFn&& Fn = {}) const&
        {
            return TTakeWhileView<TView, std::decay_t<TFn>>(static_cast<const TView&>(*this), UR_FWD(Fn));
        }

        template<typename TFn = FIdentityFunctor>
        auto TakeWhile(TFn&& Fn = {}) &&
        {
            return TTakeWhileView<TView, std::decay_t<TFn>>(static_cast<TView&&>(*this), UR_FWD(Fn));
        }
    };
}