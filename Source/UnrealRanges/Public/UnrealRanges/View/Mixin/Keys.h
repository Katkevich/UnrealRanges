#pragma once

namespace Ur::View {

    template<typename TView>
    class TKeysView;

    template<typename TView>
    struct TKeysMixin
    {
        auto Keys() const&
        {
            return TKeysView<TView>({}, static_cast<const TView&>(*this));
        }

        auto Keys() &&
        {
            return TKeysView<TView>({}, static_cast<TView&&>(*this));
        }
    };
}