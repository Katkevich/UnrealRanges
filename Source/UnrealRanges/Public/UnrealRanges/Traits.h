#pragma once
#include "UnrealRanges/View/View.h"
#include "UnrealRanges/Utility.h"
#include "Templates/IntegralConstant.h"
#include <concepts>
#include <type_traits>

namespace Ur {

    template<typename TRng>
    concept Range = HasBegin<TRng> && HasEnd<TRng>;

    template<typename TRng>
    concept BiDirRange = Range<TRng> && HasRBegin<TRng> && HasREnd<TRng>;

    template<typename TIt>
    struct IteratorTraits
    {
        // we cannot rely on iterator::reference coz UE containers may not provide it
        using reference = decltype(*std::declval<std::add_lvalue_reference_t<TIt>>()); // TIt& && -> TIt&
        using value_type = std::remove_cvref_t<reference>;
        using pointer = std::conditional_t<std::is_lvalue_reference_v<reference>, std::add_pointer_t<std::remove_reference_t<reference>>, void>;
    };

    template<typename TIt>
    using IteratorValue = typename IteratorTraits<TIt>::value_type;

    template<typename TIt>
    using IteratorReference = typename IteratorTraits<TIt>::reference;

    template<typename TIt>
    using IteratorPointer = typename IteratorTraits<TIt>::pointer;


    // we cannot rely on TRng::iterator or something similar coz UE containers may not provide it consistently
    template<Range TRng>
    using RangeIterator = decltype(Ur::Begin(std::declval<std::add_lvalue_reference_t<TRng>>())); // TRng& && -> TRng&

    template<Range TRng>
    using RangeConstIterator = decltype(Ur::Begin(std::declval<std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<TRng>>>>())); // const TRng& && -> const TRng&

    template<BiDirRange TRng>
    using RangeReverseIterator = decltype(Ur::RBegin(std::declval<std::add_lvalue_reference_t<TRng>>())); // TRng& && -> TRng&

    template<BiDirRange TRng>
    using RangeReverseConstIterator = decltype(Ur::RBegin(std::declval<std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<TRng>>>>())); // const TRng& && -> const TRng&

    template<Range TRng>
    using RangeValue = IteratorValue<RangeIterator<TRng>>;

    template<Range TRng>
    using RangeReference = IteratorReference<RangeIterator<TRng>>;

    template<Range TRng>
    using RangeConstReference = IteratorReference<RangeConstIterator<TRng>>;

    template<Range TRng>
    using RangePointer = IteratorPointer<RangeIterator<TRng>>;

    template<Range TRng>
    using RangeConstPointer = IteratorPointer<RangeConstIterator<TRng>>;


    template<typename TRng>
    concept SizedRange = Range<TRng> && requires(TRng Rng) {
        { Ur::Size(Rng) } -> std::convertible_to<int32>;
    };

    // UE random "accessness" is very limited (basically the only facility that is available is operator[])
    template<typename TRng>
    concept RandomAccessRange = SizedRange<TRng> && requires(TRng Rng, int32 Index) {
        { Rng[Index] } -> std::same_as<RangeReference<TRng>>;
    };

    template<typename TRng>
    concept ContiguousRange = RandomAccessRange<TRng> && requires(TRng Rng) {
        { Ur::Data(Rng) } -> std::same_as<RangePointer<TRng>>;
    };

    template<typename TView>
    concept UnrealView = std::derived_from<TView, Ur::View::FView>;


    template<template<typename...> typename T, template<typename...> typename U>
    struct TIsSameTemplate : TIntegralConstant<bool, false>
    {};

    template<template<typename...> typename T>
    struct TIsSameTemplate<T, T> : TIntegralConstant<bool, true>
    {};

    template<template<typename...> typename T, template<typename...> typename U>
    static constexpr bool TIsSameTemplate_V = TIsSameTemplate<T, U>::Value;


    namespace Detail
    {
        template<typename T, template<typename...> typename Tmpl>
        struct THasTemplate : TIntegralConstant<bool, false>
        {};

        template<template<typename...> typename Tmpl, typename... Ts>
        struct THasTemplate<Tmpl<Ts...>, Tmpl> : TIntegralConstant<bool, true>
        {};
    }

    template<typename T, template<typename...> typename Tmpl>
    struct THasTemplate : Detail::THasTemplate<std::remove_cvref_t<T>, Tmpl>
    {};

    template<typename T, template<typename...> typename Tmpl>
    static constexpr bool THasTemplate_V = THasTemplate<T, Tmpl>::Value;



    template<typename T>
    concept LessOrdered = requires(T Val) {
        { Val < Val } -> std::convertible_to<bool>;
    };

    template<typename T, typename TProj>
    concept LessOrderedBy = requires(T Val, TProj Proj) {
        { std::invoke(Proj, Val) < std::invoke(Proj, Val) } -> std::convertible_to<bool>;
    };

    template<typename TLhs, typename TRhs>
    concept EqComparable = requires(TLhs Lhs, TRhs Rhs) {
        { Lhs == Rhs } -> std::convertible_to<bool>;
    };

    template<typename TLhs, typename TProj, typename TRhs>
    concept EqComparableByWith = requires(TLhs Lhs, TProj Proj, TRhs Rhs) {
        { std::invoke(Proj, Lhs) == Rhs } -> std::convertible_to<bool>;
    };
}