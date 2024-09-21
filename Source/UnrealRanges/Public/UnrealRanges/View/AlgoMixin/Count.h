#pragma once
#include "UnrealRanges/View/Cursor.h"
#include "UnrealRanges/Traits.h"
#include "Templates/IdentityFunctor.h"

namespace Ur::View {

    template<typename TView>
    struct TCountMixin
    {
        // UE is defaulted to int32 so we are
        template<typename TCount = int32>
        TCount Count() const
        {
            if constexpr (SizedRange<TView>)
            {
                return TCount(Ur::Size(*static_cast<const TView*>(this)));
            }
            else
            {
                TCount ItemsCount{};
                Ur::Cursor::Iterate<Misc::Forward>(*static_cast<const TView*>(this), [&](auto&& Item)
                    {
                        ++ItemsCount;
                        return Misc::ELoop::Continue;
                    });

                return ItemsCount;
            }
        }

        // UE is defaulted to int32 so we are
        template<typename TPred = FIdentityFunctor, typename TCount = int32>
        TCount CountIf(TPred Pred = {}) const
        {
            TCount ItemsCount{};
            Ur::Cursor::Iterate<Misc::Forward>(*static_cast<const TView*>(this), [&](auto&& Item)
                {
                    if (std::invoke(Pred, UR_FWD(Item)))
                    {
                        ++ItemsCount;
                    }
                    return Misc::ELoop::Continue;
                });

            return ItemsCount;
        }
    };
}