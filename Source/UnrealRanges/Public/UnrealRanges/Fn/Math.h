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

    struct FEven
    {
        template<typename T>
        auto operator()(T&& Lhs) const requires requires { UR_FWD(Lhs) % 2; }
        {
            if constexpr (std::is_integral_v<T>)
                return (Lhs & 1) == 0;

            return Lhs % 2 == 0;
        }
    };

    struct FOdd
    {
        template<typename T>
        auto operator()(T&& Lhs) const requires requires { UR_FWD(Lhs) % 2; }
        {
            if constexpr (std::is_integral_v<T>)
                return (Lhs & 1) == 1;

            return Lhs % 2 == 1;
        }
    };

    static constexpr FPlus Plus{};
    static constexpr FMul Mul{};
    static constexpr FEven Even{};
    static constexpr FOdd Odd{};

}