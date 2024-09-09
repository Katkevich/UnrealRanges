// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FilterEmptyRange, "UnrealRanges.MinMax.TestFilterEmptyRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FilterEmptyRange::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    auto Fn = [](int32 i) { return i > 0; };
    auto Result = Ref(From).Filter(Fn);

    static_assert(std::is_same_v<TFilterView<TRefView<TArray<int32>>, decltype(Fn)>, decltype(Result)>);

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FilterEven, "UnrealRanges.MinMax.TestFilterEven", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FilterEven::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From).Filter([](int32 i) { return i % 2 == 0; });

    return EqualTo(Result, { 2,4 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FilterOutAll, "UnrealRanges.MinMax.TestFilterOutAll", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FilterOutAll::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From).Filter([](int32 i) { return i > 5; });

    return Result.begin() == Result.end();
}

#endif