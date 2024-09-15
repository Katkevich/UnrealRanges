#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"

namespace Ur::Fn
{
    struct FPlus
    {
        auto operator()(auto&& Lhs, auto&& Rhs) const requires requires { UR_FWD(Lhs) + UR_FWD(Rhs); }
        {
            return UR_FWD(Lhs) + UR_FWD(Rhs);
        }
    };

    struct FMul
    {
        auto operator()(auto&& Lhs, auto&& Rhs) const requires requires { UR_FWD(Lhs) * UR_FWD(Rhs); }
        {
            return UR_FWD(Lhs) * UR_FWD(Rhs);
        }
    };

    static constexpr FPlus Plus{};
    static constexpr FMul Mul{};
}