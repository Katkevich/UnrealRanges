#pragma once
#include "UnrealRanges/Detail/AlwaysFalse.h"
#include "UnrealRanges/Detail/ForwardMacro.h"

namespace Ur {

struct FBeginCpo
{
    template<typename TRng>
    auto operator()(TRng&& Rng) const
    {
        using std::begin;

        // some UE containers have only TRng::begin member function (TArray)
        if constexpr (requires{ UR_FWD(Rng).begin(); })
        {
            return UR_FWD(Rng).begin();
        }
        // while others have only free begin(TRng) function (TDoubleLinkedList)
        else if constexpr (requires{ begin(UR_FWD(Rng)); })
        {
            return begin(UR_FWD(Rng));
        }
        else
        {
            static_assert(Detail::FAlwaysFalse<TRng>::Value, "not a range");
        }
    }
};

struct FEndCpo
{
    template<typename TRng>
    auto operator()(TRng&& Rng) const
    {
        using std::end;

        // some UE containers have only TRng::end member function (TArray)
        if constexpr (requires{ UR_FWD(Rng).end(); })
        {
            return UR_FWD(Rng).end();
        }
        // while others have only free end(TRng) function (TDoubleLinkedList)
        else if constexpr (requires{ end(UR_FWD(Rng)); })
        {
            return end(UR_FWD(Rng));
        }
        else
        {
            static_assert(Detail::FAlwaysFalse<TRng>::Value, "not a range");
        }
    }
};

struct FSizeCpo
{
    template<typename TRng>
    auto operator()(TRng&& Rng) const
    {
        return GetNum(UR_FWD(Rng));
    }
};

struct FDataCpo
{
    template<typename TRng>
    auto operator()(TRng&& Rng) const
    {
        return GetData(UR_FWD(Rng));
    }
};

// use custom Begin & End coz UE isn't very consistent defining them (see TDoubleLinkedList which has begin & end as private friends)
static constexpr FBeginCpo Begin{};
static constexpr FEndCpo End{};
static constexpr FSizeCpo Size{};
static constexpr FDataCpo Data{};
}