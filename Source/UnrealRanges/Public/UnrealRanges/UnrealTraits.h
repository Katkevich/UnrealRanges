#pragma once
#include "UnrealRanges/Traits.h"
#include "Containers/IndirectArray.h"
#include "Containers/Map.h"
#include "Templates/IntegralConstant.h"
#include <type_traits>

namespace Ur {

    template<typename T>
    struct TIsIndirectArray : TIntegralConstant<bool, false>
    {};

    template<typename... Ts>
    struct TIsIndirectArray<TIndirectArray<Ts...>> : TIntegralConstant<bool, true>
    {};

    template<typename T>
    static constexpr bool TIsIndirectArray_V = TIsIndirectArray<T>::Value;


    template<typename T>
    struct TIsUnrealPair : TIntegralConstant<bool, false>
    {};

    template<typename TKey, typename TValue>
    struct TIsUnrealPair<TPair<TKey, TValue>> : TIntegralConstant<bool, true>
    {};

    template<typename T>
    static constexpr bool TIsUnrealPair_V = TIsUnrealPair<T>::Value;


    // customization point
    template<template <typename...> typename UMap>
    struct TMapTraits
    {
        static constexpr bool IsMap = false;
    };

    template<>
    struct TMapTraits<TMap>
    {
        static constexpr bool IsMap = true;
    };

    template<>
    struct TMapTraits<TMultiMap>
    {
        static constexpr bool IsMap = true;
    };

    template<template<typename, typename, typename... Ts> typename T>
    static constexpr bool TIsMapTemplate_V = TMapTraits<T>::IsMap;
}