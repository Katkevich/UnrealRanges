#pragma once

namespace Ur::View {

    template<typename TView>
    class TReverseView;

    template<typename TView>
    struct TReverseMixin
    {
        auto Reverse()
        {
            return TReverseView<TView>({}, static_cast<TView&>(*this));
        }
    };
}