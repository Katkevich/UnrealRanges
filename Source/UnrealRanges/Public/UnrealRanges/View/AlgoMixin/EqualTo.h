#pragma once
#include "UnrealRanges/View/Cursor.h"
#include "UnrealRanges/Traits.h"

namespace Ur::View {
    namespace Detail
    {
        template<typename TView>
        struct TEqualToMixin
        {
        protected:
            template<typename TSelf, typename TRng>
            static auto EqualTo(TSelf& Self, TRng& Rng)
            {
                if constexpr (SizedRange<TSelf> && SizedRange<TRng>)
                {
                    if (Ur::Size(Self) != Ur::Size(Rng))
                        return false;
                }

                bool bIsEq = true;

                auto RngIt = Ur::Begin(Rng);
                auto RngEndIt = Ur::End(Rng);

                Ur::Cursor::Iterate<Misc::Forward>(Self, [&](auto&& Item)
                    {
                        if (RngIt == RngEndIt)
                        {
                            bIsEq = false;
                            return Ur::ELoop::Break;
                        }
                        else if (Item != *RngIt)
                        {
                            bIsEq = false;
                            return Ur::ELoop::Break;
                        }
                        else
                        {
                            ++RngIt;
                            return Ur::ELoop::Continue;
                        }
                    });

                return bIsEq && RngIt == RngEndIt;
            }
        };
    }

    template<typename TView>
    struct TEqualToMixin : Detail::TEqualToMixin<TView>
    {
    public:
        template<Ur::Range TRng>
        auto EqualTo(TRng&& Rng)
        {
            return Detail::TEqualToMixin<TView>::EqualTo(*static_cast<TView*>(this), Rng);
        }

        template<Ur::Range TRng>
        auto EqualTo(TRng&& Rng) const
        {
            return Detail::TEqualToMixin<TView>::EqualTo(*static_cast<const TView*>(this), Rng);
        }
    };
}