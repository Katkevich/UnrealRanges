#pragma once
#include "UnrealRanges/Detail/AlwaysFalse.h"
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "Templates/IntegralConstant.h"
#include <concepts>

namespace Ur {
    template<typename T>
    struct TOwned;

    template<typename T>
    struct TOwned<T*>
    {
        using ElementType = T;

        T* Ptr{};
    };

    template<typename T>
    auto Owned(T* Ptr)
    {
        return TOwned<T*>{ Ptr };
    }

    template<typename T>
    struct TIsOwned : TIntegralConstant<bool, false>
    {};

    template<typename T>
    struct TIsOwned<TOwned<T*>> : TIntegralConstant<bool, true>
    {};

    template<typename T>
    static constexpr bool TIsOwned_V = TIsOwned<T>::Value;
}