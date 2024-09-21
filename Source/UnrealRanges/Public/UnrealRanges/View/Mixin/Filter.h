#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"
#include <type_traits>

namespace Ur::View {

    template<typename TView, typename TFn>
    class TFilterView;

    template<typename TView>
    struct TFilterMixin
    {
        template<typename TFn>
        auto Filter(TFn&& Fn) const&
        {
            return TFilterView<TView, std::decay_t<TFn>>(static_cast<const TView&>(*this), UR_FWD(Fn));
        }

        template<typename TFn>
        auto Filter(TFn&& Fn) &&
        {
            return TFilterView<TView, std::decay_t<TFn>>(static_cast<TView&&>(*this), UR_FWD(Fn));
        }
    };
}