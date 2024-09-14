#pragma once

namespace Ur::Fn
{
    struct FPlus
    {
        auto operator()(auto&& Lhs, auto&& Rhs) const
        {
            return Lhs + Rhs;
        }
    };

    struct FMul
    {
        auto operator()(auto&& Lhs, auto&& Rhs) const
        {
            return Lhs * Rhs;
        }
    };

    static constexpr FPlus Plus{};
    static constexpr FMul Mul{};
}