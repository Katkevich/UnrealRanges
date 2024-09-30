#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "Templates/Tuple.h"
#include <tuple>

namespace Ur::Fn
{
    struct FKey
    {
        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(const TTuple<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Lhs>)
        {
            return (Tuple.Key);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(TTuple<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Lhs>)
        {
            return (Tuple.Key);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(TTuple<Lhs, Rhs>&& Tuple) const requires (!std::is_rvalue_reference_v<Lhs>)
        {
            if constexpr (std::is_lvalue_reference_v<Lhs>)
                return (Tuple.Key);
            else
                return Tuple.Key;
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(const std::pair<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Lhs>)
        {
            return (Tuple.first);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(std::pair<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Lhs>)
        {
            return (Tuple.first);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(std::pair<Lhs, Rhs>&& Tuple) const requires (!std::is_rvalue_reference_v<Lhs>)
        {
            if constexpr (std::is_lvalue_reference_v<Lhs>)
                return (Tuple.first);
            else
                return Tuple.first;
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(const std::tuple<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Lhs>)
        {
            return std::get<0>(Tuple);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(std::tuple<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Lhs>)
        {
            return std::get<0>(Tuple);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(std::tuple<Lhs, Rhs>&& Tuple) const requires (!std::is_rvalue_reference_v<Lhs>)
        {
            if constexpr (std::is_lvalue_reference_v<Lhs>)
                return std::get<0>(Tuple);
            else
                return Lhs(MoveTemp(std::get<0>(Tuple))); // explicit move
        }
    };

    struct FValue
    {
        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(const TTuple<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Rhs>)
        {
            return (Tuple.Value);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(TTuple<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Rhs>)
        {
            return (Tuple.Value);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(TTuple<Lhs, Rhs>&& Tuple) const requires (!std::is_rvalue_reference_v<Rhs>)
        {
            if constexpr (std::is_lvalue_reference_v<Rhs>)
                return (Tuple.Value);
            else
                return Tuple.Value;
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(const std::pair<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Rhs>)
        {
            return (Tuple.second);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(std::pair<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Rhs>)
        {
            return (Tuple.second);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(std::pair<Lhs, Rhs>&& Tuple) const requires (!std::is_rvalue_reference_v<Rhs>)
        {
            if constexpr (std::is_lvalue_reference_v<Rhs>)
                return (Tuple.second);
            else
                return Tuple.second;
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(const std::tuple<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Rhs>)
        {
            return std::get<1>(Tuple);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(std::tuple<Lhs, Rhs>& Tuple) const requires (!std::is_rvalue_reference_v<Rhs>)
        {
            return std::get<1>(Tuple);
        }

        template<typename Lhs, typename Rhs>
        decltype(auto) operator()(std::tuple<Lhs, Rhs>&& Tuple) const requires (!std::is_rvalue_reference_v<Rhs>)
        {
            if constexpr (std::is_lvalue_reference_v<Rhs>)
                return std::get<1>(Tuple);
            else
                return Rhs(MoveTemp(std::get<1>(Tuple))); // explicit move
        }
    };

    static constexpr FKey Key{};
    static constexpr FValue Value{};
}