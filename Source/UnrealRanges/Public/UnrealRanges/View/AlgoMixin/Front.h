#pragma once
#include "UnrealRanges/View/Cursor.h"
#include "UnrealRanges/Optional.h"

namespace Ur::View {

    template<typename TView>
    struct TFrontMixin
    {
        auto Front() const
        {
            using TOptValueType = TOptional<TReference<const TView>>;

            auto& Self = *static_cast<const TView*>(this);
            auto Curs = Ur::Cursor::Begin<Misc::Forward>(Self);

            if (!Ur::Cursor::IsEnd(Self, Curs))
                return TOptValueType(Ur::Cursor::Deref(Self, Curs));
            else
                return TOptValueType{};
        }

        auto Front()
        {
            using TOptValueType = TOptional<TReference<TView>>;

            auto& Self = *static_cast<TView*>(this);
            auto Curs = Ur::Cursor::Begin<Misc::Forward>(Self);

            if (!Ur::Cursor::IsEnd(Self, Curs))
                return TOptValueType(Ur::Cursor::Deref(Self, Curs));
            else
                return TOptValueType{};
        }
    };
}