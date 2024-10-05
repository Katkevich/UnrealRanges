// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_IotaFollowedByTake, "UnrealRanges.Iota.TestIotaFollowedByTake", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_IotaFollowedByTake::RunTest(const FString& Parameters)
{
    auto Result = Iota(0).Take(5);

    static_assert(std::is_same_v<TTakeView<TIotaView<int32>>, decltype(Result)>);
    static_assert(std::is_same_v<int32, decltype(*Result.begin())>);

    return EqualTo(Result, { 0,1,2,3,4 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_IotaIsNotEmpty, "UnrealRanges.Iota.TestIotaIsNotEmpty", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_IotaIsNotEmpty::RunTest(const FString& Parameters)
{
    auto Result = Iota(0);

    static_assert(std::is_same_v<TIotaView<int32>, decltype(Result)>);

    return !Result.IsEmpty();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_IotaInternalIterationForeach, "UnrealRanges.Iota.TestIotaInternalIterationForeach", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_IotaInternalIterationForeach::RunTest(const FString& Parameters)
{
    TArray<int32> Nums;
    Iota(0, 2).Foreach([&](int32 Num) { Nums.Add(Num); return Num > 8 ? Ur::ELoop::Break : Ur::ELoop::Continue; });

    return EqualTo(Nums, { 0,2,4,6,8,10 });
}

#endif