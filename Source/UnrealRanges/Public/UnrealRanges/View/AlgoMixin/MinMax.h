#pragma once
#include "UnrealRanges/Detail/CursorProtocol.h"
#include "UnrealRanges/Optional.h"
#include "UnrealRanges/Traits.h"
#include "Templates/IdentityFunctor.h"

namespace Ur {
    template<typename T>
    struct TMinMax
    {
        TOptional<T> Min{};
        TOptional<T> Max{};
    };
}
namespace Ur::View {

    template<typename TView>
    struct TMinMaxMixin
    {
        template<typename TProj = FIdentityFunctor>
        auto Min(TProj Proj = {})
        {
            return TMinMaxMixin::MinImpl(std::type_identity<typename TView::reference>{}, static_cast<TView*>(this), Proj);
        }

        template<typename TProj = FIdentityFunctor>
        auto Min(TProj Proj = {}) const
        {
            return TMinMaxMixin::MinImpl(std::type_identity<typename TView::const_reference>{}, static_cast<const TView*>(this), Proj);
        }

        template<typename TProj = FIdentityFunctor>
        auto Max(TProj Proj = {})
        {
            return TMinMaxMixin::MaxImpl(std::type_identity<typename TView::reference>{}, static_cast<TView*>(this), Proj);
        }

        template<typename TProj = FIdentityFunctor>
        auto Max(TProj Proj = {}) const
        {
            return TMinMaxMixin::MaxImpl(std::type_identity<typename TView::const_reference>{}, static_cast<const TView*>(this), Proj);
        }

        template<typename TProj = FIdentityFunctor>
        auto MinMax(TProj Proj = {})
        {
            return TMinMaxMixin::MinMaxImpl(std::type_identity<typename TView::reference>{}, static_cast<TView*>(this), Proj);
        }

        template<typename TProj = FIdentityFunctor>
        auto MinMax(TProj Proj = {}) const
        {
            return TMinMaxMixin::MinMaxImpl(std::type_identity<typename TView::const_reference>{}, static_cast<const TView*>(this), Proj);
        }

    private:
        template<typename TRef, typename TSelf, typename TProj>
        static auto MinImpl(std::type_identity<TRef>, TSelf Self, TProj Proj)
        {
            static_assert(LessOrderedBy<TRef, TProj>, "items (of projected items if projection is used) should be comparable using operator<");

            TOptional<TRef> MinItem{};
            FCursorProtocol::InternalIteration(Misc::Forward, *Self, [&](auto&& Item)
                {
                    if (!MinItem || std::invoke(Proj, Item) < std::invoke(Proj, *MinItem))
                    {
                        MinItem = UR_FWD(Item);
                    }
                    return Misc::ELoop::Continue;
                });

            return MinItem;
        }

        template<typename TRef, typename TSelf, typename TProj>
        static auto MaxImpl(std::type_identity<TRef>, TSelf Self, TProj Proj)
        {
            static_assert(LessOrderedBy<TRef, TProj>, "items (of projected items if projection is used) should be comparable using operator<");

            TOptional<TRef> MaxItem{};
            FCursorProtocol::InternalIteration(Misc::Forward, *Self, [&](auto&& Item)
                {
                    if (!MaxItem || std::invoke(Proj, *MaxItem) < std::invoke(Proj, Item))
                    {
                        MaxItem = UR_FWD(Item);
                    }
                    return Misc::ELoop::Continue;
                });

            return MaxItem;
        }

        template<typename TRef, typename TSelf, typename TProj>
        static auto MinMaxImpl(std::type_identity<TRef>, TSelf Self, TProj Proj)
        {
            static_assert(LessOrderedBy<TRef, TProj>, "items (of projected items if projection is used) should be comparable using operator<");

            TMinMax<TRef> MinMaxPair{};
            FCursorProtocol::InternalIteration(Misc::Forward, *Self, [&](auto&& Item)
                {
                    if (!MinMaxPair.Max)
                    {
                        MinMaxPair.Min = Item;
                        MinMaxPair.Max = Item;
                    }
                    else if (std::invoke(Proj, Item) < std::invoke(Proj, *MinMaxPair.Min))
                    {
                        MinMaxPair.Min = UR_FWD(Item);
                    }
                    else if (std::invoke(Proj, *MinMaxPair.Max) < std::invoke(Proj, Item))
                    {
                        MinMaxPair.Max = UR_FWD(Item);
                    }
                    return Misc::ELoop::Continue;
                });

            return MinMaxPair;
        }
    };
}