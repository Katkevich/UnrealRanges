#pragma once
#include "UnrealRanges/View/View.h"
#include "UnrealRanges/Utility.h"
#include "Templates/IntegralConstant.h"
#include <concepts>
#include <type_traits>

namespace Ur {

    template<typename TRng>
    concept Range = requires(TRng Rng) {
        Ur::Begin(Rng);
        Ur::End(Rng);
    };

    template<typename TRng>
    concept BiDirRange = Range<TRng> && requires(TRng Rng) {
        Ur::RBegin(Rng);
        Ur::REnd(Rng);
    };

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
        { Ur::Size(Rng) } -> std::same_as<int32>;
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
}