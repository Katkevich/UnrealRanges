#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "UnrealRanges/Traits.h"
#include <type_traits>
#include <tuple>

namespace Ur::View {

    template<Ur::RangeView TUnderlView, std::integral TIndex>
    class TEnumerateView;

    template<typename T, std::integral TIndex = int32>
    struct FIndexed
    {
    private:
        using reference = std::add_lvalue_reference_t<std::remove_const_t<std::remove_cvref_t<T>>>;

    public:
        static_assert(!std::is_rvalue_reference_v<T>);

        FIndexed() requires !std::is_reference_v<T> = default;
        FIndexed(const FIndexed&) = default;
        FIndexed(FIndexed&&) = default;
        FIndexed& operator=(const FIndexed&) = default;
        FIndexed& operator=(FIndexed&&) = default;

        template<typename U>
        FIndexed(U&& InValue, TIndex InIndex)
                noexcept(std::is_nothrow_constructible_v<T, decltype(InValue)>)
            : Value(UR_FWD(InValue))
            , Index(InIndex)
        {
        }

        // FIndexed<T&> -> FIndexed<const T&>
        // FIndexed<T&> -> FIndexed<T>
        FIndexed(FIndexed<reference, TIndex>&& That)
                noexcept(std::is_nothrow_constructible_v<T, decltype(That.Value)>)
                requires(std::is_constructible_v<T, reference> && !std::is_same_v<T, reference>)
            : Value(UR_FWD(That.Value))
            , Index(That.Index)
        {
        }

        auto Tie() const
        {
            return std::tie(Value, Index);
        }

        template <std::size_t I>
        decltype(auto) get() const&
        {
            if constexpr (I == 0)
                return (Value);
            else if constexpr (I == 1)
                return (Index);
        }

        template <std::size_t I>
        decltype(auto) get() &
        {
            if constexpr (I == 0)
                return (Value);
            else if constexpr (I == 1)
                return (Index);
        }

        // repeating TTuple interface "tuple" protocol
        // see EnumerateTests.cpp|TestEnumerateStructuredBinding for details
        template <std::size_t I>
        decltype(auto) get() &&
        {
            if constexpr (I == 0)
            {
                if constexpr (std::is_reference_v<T>)
                    return (Value);
                else
                    return static_cast<T&&>(Value);
            }
            else if constexpr (I == 1)
            {
                if constexpr (std::is_reference_v<TIndex>)
                    return (Index);
                else
                    return static_cast<TIndex&&>(Index);
            }
        }

        template <std::size_t I>
        decltype(auto) get() const&&
        {
            if constexpr (I == 0)
            {
                if constexpr (std::is_reference_v<T>)
                    return (Value);
                else
                    return static_cast<T&&>(Value);
            }
            else if constexpr (I == 1)
            {
                if constexpr (std::is_reference_v<TIndex>)
                    return (Index);
                else
                    return static_cast<TIndex&&>(Index);
            }
        }

        T Value{};
        TIndex Index{};

    private:
        friend bool operator<(const FIndexed<T, TIndex>& Lhs, const FIndexed<T, TIndex>& Rhs)
        {
            return Lhs.Tie() < Rhs.Tie();
        }

        friend bool operator==(const FIndexed<T, TIndex>& Lhs, const FIndexed<T, TIndex>& Rhs)
        {
            return Lhs.Tie() == Rhs.Tie();
        }
    };

    template<typename TView>
    struct TEnumerateMixin
    {
        template<typename TIndex = int32>
        auto Enumerate(TIndex StartFrom = { 0 }, TIndex Step = { 1 }) const&
        {
            return TEnumerateView<TView, TIndex>(static_cast<const TView&>(*this), StartFrom, Step);
        }

        template<typename TIndex = int32>
        auto Enumerate(TIndex StartFrom = { 0 }, TIndex Step = { 1 }) &&
        {
            return TEnumerateView<TView, TIndex>(static_cast<TView&&>(*this), StartFrom, Step);
        }
    };
}

namespace std {
    template <typename T, typename TIndex>
    struct tuple_size<Ur::View::FIndexed<T, TIndex>> : std::integral_constant<std::size_t, 2> {};

    template <typename T, typename TIndex>
    struct tuple_element<0, Ur::View::FIndexed<T, TIndex>> {
        using type = T;
    };

    template <typename T, typename TIndex>
    struct tuple_element<1, Ur::View::FIndexed<T, TIndex>> {
        using type = TIndex;
    };
}