// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Iota, TestIotaFollowedByTake)
{
    auto Result = Iota(0).Take(5);

    static_assert(std::is_same_v<TTakeView<TIotaView<int32>>, decltype(Result)>);
    static_assert(std::is_same_v<int32, decltype(*Result.begin())>);

    return EqualTo(Result, { 0,1,2,3,4 });
}

UR_TEST(UnrealRanges, Iota, TestIotaIsNotEmpty)
{
    auto Result = Iota(0);

    static_assert(std::is_same_v<TIotaView<int32>, decltype(Result)>);

    return !Result.IsEmpty();
}

UR_TEST(UnrealRanges, Iota, TestIotaInternalIterationForeach)
{
    TArray<int32> Nums;
    Iota(0, 2).Foreach([&](int32 Num) { Nums.Add(Num); return Num > 8 ? Ur::ELoop::Break : Ur::ELoop::Continue; });

    return EqualTo(Nums, { 0,2,4,6,8,10 });
}

#endif