#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "UnrealRanges/Traits.h"
#include <type_traits>

namespace Ur::View {

    template<Ur::RangeView TView, std::regular_invocable<typename TView::reference> TFn>
    class TTransformView;

    template<typename TView>
    struct TTransformMixin
    {
        template<typename TFn>
        auto Transform(TFn&& Fn) const&
        {
            return TTransformView<TView, std::decay_t<TFn>>(static_cast<const TView&>(*this), UR_FWD(Fn));
        }

        template<typename TFn>
        auto Transform(TFn&& Fn) &&
        {
            return TTransformView<TView, std::decay_t<TFn>>(static_cast<TView&&>(*this), UR_FWD(Fn));
        }
    };
}