#pragma once
#include "UnrealRanges/View/Cursor.h"
#include "UnrealRanges/Traits.h"

namespace Ur::View {
    namespace Detail {

        template<typename TView>
        struct TStartsWithMixin
        {
        protected:
            template<typename TSelf, typename TRng>
            static auto StartsWith(TSelf& Self, TRng& Rng)
            {
                if constexpr (SizedRange<TSelf> && SizedRange<TRng>)
                {
                    if (Ur::Size(Self) < Ur::Size(Rng))
                        return false;
                }

                auto RngIt = Ur::Begin(Rng);
                auto RngEndIt = Ur::End(Rng);

                Ur::Cursor::Iterate<Misc::Forward>(Self, [&](auto&& Item)
                    {
                        if (RngIt == RngEndIt)
                        {
                            return Ur::ELoop::Break;
                        }
                        else if (Item != *RngIt)
                        {
                            return Ur::ELoop::Break;
                        }
                        else
                        {
                            ++RngIt;
                            return Ur::ELoop::Continue;
                        }
                    });

                return RngIt == RngEndIt;
            }
        };
    }

    template<typename TView>
    struct TStartsWithMixin : Detail::TStartsWithMixin<TView>
    {
    public:
        template<Ur::Range TRng>
        auto StartsWith(TRng& Rng)
        {
            return Detail::TStartsWithMixin<TView>::StartsWith(*static_cast<TView*>(this), Rng);
        }

        template<Ur::Range TRng>
        auto StartsWith(TRng& Rng) const
        {
            return Detail::TStartsWithMixin<TView>::StartsWith(*static_cast<const TView*>(this), Rng);
        }
    };
}