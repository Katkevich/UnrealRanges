#pragma once
#include "UnrealRanges/Detail/CursorProtocol.h"
#include "UnrealRanges/Fn/Math.h"
#include "Templates/IdentityFunctor.h"

namespace Ur::View {

    template<typename TView>
    struct TSumMixin
    {
        template<typename TProj = FIdentityFunctor>
        auto Sum(TProj Proj = {}) const
        {
            using TRef = typename TView::const_reference;

            static_assert(std::is_invocable_v<TProj, TRef>, "Projection should be invokable over range Item");
            using TItem = std::invoke_result_t<TProj, TRef>;

            static_assert(std::is_invocable_v<Ur::Fn::FPlus, TItem, TItem>, "missing 'operator+(Proj(Item), Proj(Item)'");
            using ResultType = std::invoke_result_t<Ur::Fn::FPlus, TItem, TItem>;

            static_assert(std::is_invocable_r_v<ResultType, Ur::Fn::FPlus, ResultType, TItem>, "missing 'operator+(<result_of>[Proj(Item) + Proj(Item)], Proj(Item))'");
            static_assert(!std::is_reference_v<ResultType>, "cannot be a reference");

            ResultType Result{};

            FCursorProtocol::InternalIteration<Misc::Forward>(*static_cast<const TView*>(this), [&](auto&& Item)
                {
                    Result = Result + std::invoke(Proj, UR_FWD(Item));

                    return Misc::ELoop::Continue;
                });

            return Result;
        }
    };
}