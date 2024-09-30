#pragma once
#include "UnrealRanges/Traits.h"
#include "Containers/IndirectArray.h"
#include "Containers/StaticArray.h"
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
    struct TIsStaticArray : TIntegralConstant<bool, false>
    {};

    template<typename T, uint32 ArrSize, uint32 Alignment>
    struct TIsStaticArray<TStaticArray<T, ArrSize, Alignment>> : TIntegralConstant<bool, true>
    {};

    template<typename T>
    static constexpr bool TIsStaticArray_V = TIsStaticArray<T>::Value;


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

    namespace Detail {
        template<typename T>
        struct TIsPair : TIntegralConstant<bool, false>
        {};

        template<typename T1, typename T2>
        struct TIsPair<TTuple<T1, T2>> : TIntegralConstant<bool, true>
        {};

        template<typename T1, typename T2>
        struct TIsPair<std::tuple<T1, T2>> : TIntegralConstant<bool, true>
        {};

        template<typename T1, typename T2>
        struct TIsPair<std::pair<T1, T2>> : TIntegralConstant<bool, true>
        {};
    }

    template<typename T>
    struct TIsPair : Detail::TIsPair<std::remove_cvref_t<T>>
    {};

    template<typename T>
    static constexpr bool TIsPair_V = TIsPair<T>::Value;
}