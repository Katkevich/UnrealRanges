#pragma once
#include "UnrealRanges/View/Cursor.h"

namespace Ur::View {

    template<typename TView>
    struct TBackMixin
    {
        auto Back() const
        {
            using TOptValueType = TOptional<TReference<const TView>>;

            auto& Self = *static_cast<const TView*>(this);
            auto Curs = Ur::Cursor::Begin<Misc::Reverse>(Self);

            if (!Ur::Cursor::IsEnd(Self, Curs))
                return TOptValueType(Ur::Cursor::Deref(Self, Curs));
            else
                return TOptValueType{};
        }

        auto Back()
        {
            using TOptValueType = TOptional<TReference<TView>>;

            auto& Self = *static_cast<TView*>(this);
            auto Curs = Ur::Cursor::Begin<Misc::Reverse>(Self);

            if (!Ur::Cursor::IsEnd(Self, Curs))
                return TOptValueType(Ur::Cursor::Deref(Self, Curs));
            else
                return TOptValueType{};
        }
    };
}