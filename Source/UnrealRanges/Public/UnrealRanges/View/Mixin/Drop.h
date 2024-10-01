#pragma once

namespace Ur::View {

    template<typename TView, typename TFn>
    class TDropView;

    template<typename TView>
    struct TDropMixin
    {
        template<typename TAmount = int32>
        auto Drop(TAmount Amount) const&
        {
            return TDropView<TView, TAmount>(static_cast<const TView&>(*this), Amount);
        }

        template<typename TAmount = int32>
        auto Drop(TAmount Amount) &&
        {
            return TDropView<TView, TAmount>(static_cast<TView&&>(*this), Amount);
        }
    };
}