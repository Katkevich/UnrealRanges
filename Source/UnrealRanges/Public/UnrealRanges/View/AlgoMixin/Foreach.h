#pragma once
#include "UnrealRanges/View/Cursor.h"
#include "UnrealRanges/View/View.h"
#include <type_traits>

namespace Ur::View {
    namespace Detail {

        template<typename TView>
        struct TForeachMixin
        {
            template<typename TSelf, typename TFn>
            static void Foreach(TSelf& Self, TFn& Fn)
            {
                static_assert(std::is_invocable_v<TFn, TReference<TSelf>>, "Projection should be invokable over range Item");
                using TItem = std::invoke_result_t<TFn, TReference<TSelf>>;

                static_assert(std::is_void_v<TItem> || std::is_same_v<TItem, Ur::ELoop>, "Function you are passing into 'Foreach' must return nothing (void) or ELoop enumeration (allow to to break or continue the loop)");

                if constexpr (std::is_void_v<TItem>)
                {
                    Ur::Cursor::Iterate<Misc::Forward>(Self, [&](auto&& Item)
                        {
                            std::invoke(Fn, UR_FWD(Item));

                            return Ur::ELoop::Continue;
                        });
                }
                else if constexpr (std::is_same_v<TItem, Ur::ELoop>)
                {
                    Ur::Cursor::Iterate<Misc::Forward>(Self, [&](auto&& Item)
                        {
                            return std::invoke(Fn, UR_FWD(Item));
                        });
                }
            }
        };
    }

    template<typename TView>
    struct TForeachMixin
    {
        template<typename TFn>
        auto Foreach(TFn Fn) const
        {
            return Detail::TForeachMixin<TView>::Foreach(*static_cast<const TView*>(this), Fn);
        }

        template<typename TFn>
        auto Foreach(TFn Fn)
        {
            return Detail::TForeachMixin<TView>::Foreach(*static_cast<TView*>(this), Fn);
        }
    };
}