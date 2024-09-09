#pragma once

namespace Ur::View {

    template<typename TView, typename TFn>
    class TFilterView;

    template<typename TView>
    struct TFilterMixin
    {
        template<typename TFn>
        auto Filter(TFn Fn) const
        {
            return TFilterView<TView, TFn>(static_cast<const TView&>(*this), Fn);
        }
    };
}