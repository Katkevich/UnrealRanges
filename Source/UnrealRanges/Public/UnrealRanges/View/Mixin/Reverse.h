#pragma once

namespace Ur::View {

    template<typename TView>
    class TReverseView;

    template<typename TView>
    struct TReverseMixin
    {
        auto Reverse() const&
        {
            return TReverseView<TView>({}, static_cast<const TView&>(*this));
        }

        auto Reverse() &&
        {
            return TReverseView<TView>({}, static_cast<TView&&>(*this));
        }
    };
}