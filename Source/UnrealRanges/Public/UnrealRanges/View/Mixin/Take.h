#pragma once

namespace Ur::View {

    template<typename TView, typename TFn>
    class TTakeView;

    template<typename TView>
    struct TTakeMixin
    {
        template<typename TAmount = int32>
        auto Take(TAmount Amount) const
        {
            return TTakeView<TView, TAmount>(static_cast<const TView&>(*this), Amount);
        }
    };
}