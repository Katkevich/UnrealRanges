#pragma once

namespace Ur {
    namespace Detail {
        template<typename T>
        struct FEmptyBase
        {
        };
    }

    template<bool Condition, typename T>
    using TConditionalInheritance = std::conditional_t<Condition, T, Detail::FEmptyBase<T>>;
}