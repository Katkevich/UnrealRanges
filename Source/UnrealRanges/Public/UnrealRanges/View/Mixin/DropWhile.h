#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"
#include <type_traits>

namespace Ur::View {

    template<typename TView, typename TFn>
    class TDropWhileView;

    template<typename TView>
    struct TDropWhileMixin
    {
        template<typename TFn>
        auto DropWhile(TFn&& Fn) const&
        {
            return TDropWhileView<TView, std::decay_t<TFn>>(static_cast<const TView&>(*this), UR_FWD(Fn));
        }

        template<typename TFn>
        auto DropWhile(TFn&& Fn) &&
        {
            return TDropWhileView<TView, std::decay_t<TFn>>(static_cast<TView&&>(*this), UR_FWD(Fn));
        }
    };
}