#pragma once
#include "UnrealRanges/Traits.h"

namespace Ur::View {

    template<typename TView>
    struct TSizeMixin
    {
        auto Num() const
        {
            return Ur::Cursor::BaseViewNum(*static_cast<const TView*>(this));
        }
    };
}