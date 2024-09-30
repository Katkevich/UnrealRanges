#pragma once

namespace Ur::View {

    template<typename TView>
    class TValuesView;

    template<typename TView>
    struct TValuesMixin
    {
        auto Values() const&
        {
            return TValuesView<TView>({}, static_cast<const TView&>(*this));
        }

        auto Values()&&
        {
            return TValuesView<TView>({}, static_cast<TView&&>(*this));
        }
    };
}