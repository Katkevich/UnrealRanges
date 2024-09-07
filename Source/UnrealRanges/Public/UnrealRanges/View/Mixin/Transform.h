#pragma once

namespace Ur::View {

    template<typename TView, typename TFn>
    class TTransformView;

    template<typename TView>
    struct TTransformMixin
    {
        template<typename TFn>
        auto Transform(TFn Fn)
        {
            return TTransformView<TView, TFn>(static_cast<TView&>(*this), Fn);
        }
    };
}