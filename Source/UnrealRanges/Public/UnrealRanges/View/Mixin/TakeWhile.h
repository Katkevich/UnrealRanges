#pragma once
#include "Templates/IdentityFunctor.h"

namespace Ur::View {

    template<typename TView, typename TFn>
    class TTakeWhileView;

    template<typename TView>
    struct TTakeWhileMixin
    {
        template<typename TFn = FIdentityFunctor>
        auto TakeWhile(TFn Fn = {}) const
        {
            return TTakeWhileView<TView, TFn>(static_cast<const TView&>(*this), Fn);
        }
    };
}