#pragma once
#include "UnrealRanges/View/Cursor.h"
#include "UnrealRanges/Fn/Math.h"
#include "Templates/IdentityFunctor.h"

namespace Ur::View {
    namespace Detail {

        template<typename TView>
        struct TSumMixin
        {
            template<typename TSelf, typename TProj>
            static auto Sum(TSelf& Self, TProj& Proj)
            {
                static_assert(std::is_invocable_v<TProj, TReference<TSelf>>, "Projection should be invokable over range Item");
                using TItem = std::invoke_result_t<TProj, TReference<TSelf>>;

                static_assert(std::is_invocable_v<Ur::Fn::FPlus, TItem, TItem>, "missing 'operator+(Proj(Item), Proj(Item)'");
                using ResultType = std::invoke_result_t<Ur::Fn::FPlus, TItem, TItem>;

                static_assert(std::is_invocable_r_v<ResultType, Ur::Fn::FPlus, ResultType, TItem>, "missing 'operator+(<result_of>[Proj(Item) + Proj(Item)], Proj(Item))'");
                static_assert(!std::is_reference_v<ResultType>, "cannot be a reference");

                ResultType Result{};

                Ur::Cursor::Iterate<Misc::Forward>(Self, [&](auto&& Item)
                    {
                        Result = Result + std::invoke(Proj, UR_FWD(Item));

                        return Misc::ELoop::Continue;
                    });

                return Result;
            }
        };
    }

    template<typename TView>
    struct TSumMixin
    {
        template<typename TProj = FIdentityFunctor>
        auto Sum(TProj Proj = {}) const
        {
            return Detail::TSumMixin<TView>::Sum(*static_cast<const TView*>(this), Proj);
        }

        template<typename TProj = FIdentityFunctor>
        auto Sum(TProj Proj = {})
        {
            return Detail::TSumMixin<TView>::Sum(*static_cast<TView*>(this), Proj);
        }
    };
}