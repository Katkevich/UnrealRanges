#pragma once
#include "UnrealRanges/View/Cursor.h"
#include "UnrealRanges/Traits.h"
#include "Templates/IdentityFunctor.h"

namespace Ur::View {
    namespace Detail {
        template<typename TView>
        struct TCountMixin
        {
            // UE is defaulted to int32 so we are
            template<typename TSelf, typename TPred, typename TCount>
            static TCount CountIf(TSelf& Self, TPred& Pred)
            {
                static_assert(std::is_invocable_v<TPred, TReference<TSelf>>, "Projection should be invokable over range Item");

                TCount ItemsCount{};
                Ur::Cursor::Iterate<Misc::Forward>(Self, [&](auto&& Item)
                    {
                        if (std::invoke(Pred, UR_FWD(Item)))
                        {
                            ++ItemsCount;
                        }
                        return Ur::ELoop::Continue;
                    });

                return ItemsCount;
            }
        };
    }


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
                        return Ur::ELoop::Continue;
                    });

                return ItemsCount;
            }
        }

        template<typename TPred = FIdentityFunctor, typename TCount = int32>
        TCount CountIf(TPred Pred = {}) const
        {
            return Detail::TCountMixin<TView>::template CountIf<const TView, TPred, TCount>(*static_cast<const TView*>(this), Pred);
        }

        template<typename TPred = FIdentityFunctor, typename TCount = int32>
        TCount CountIf(TPred Pred = {})
        {
            return Detail::TCountMixin<TView>::template CountIf<TView, TPred, TCount>(*static_cast<TView*>(this), Pred);
        }
    };
}