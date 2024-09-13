// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeEmptyRange, "UnrealRanges.Take.TestTakeEmptyRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeEmptyRange::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    auto Result = Ref(From).Take(10);

    static_assert(std::is_same_v<TTakeView<TRefView<TArray<int32>>, int32>, decltype(Result)>);

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeGettingMoreThanThereIs, "UnrealRanges.Take.TestTakeGettingMoreThanThereIs", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeGettingMoreThanThereIs::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3 };

    const auto Result = Ref(From).Take(10);

    return EqualTo(Result, { 1, 2, 3});
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeGettingLessThanThereIs, "UnrealRanges.Take.TestTakeGettingLessThanThereIs", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeGettingLessThanThereIs::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    const auto Result = Ref(From).Take(3);

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeGettingZeroElements, "UnrealRanges.Take.TestTakeGettingZeroElements", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeGettingZeroElements::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    const auto Result = Ref(From).Take(0);

    return Result.cbegin() == Result.cend();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeWithOtherViewsInternalIteration, "UnrealRanges.Take.TestTakeWithOtherViewsInternalIteration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeWithOtherViewsInternalIteration::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5,6,7,8,9 };

    const auto Result = Ref(From)
        .Filter([](int32 Num) { return Num % 2 == 0; })
        .Transform([](int32 Num) { return Num * Num; })
        .Reverse()
        .Take(3)
        .Enumerate(1)
        .To<TArray>();

    return EqualTo(Result, { FIndexed<int32>(64, 1), FIndexed<int32>(36, 2), FIndexed<int32>(16, 3) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeWithOtherViews, "UnrealRanges.Take.TestTakeWithOtherViews", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeWithOtherViews::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5,6,7,8,9 };

    const auto Result = Ref(From)
        .Filter([](int32 Num) { return Num % 2 == 0; })
        .Transform([](int32 Num) { return Num * Num; })
        .Reverse()
        .Take(3)
        .Enumerate(1);

    return EqualTo(Result, { FIndexed<int32>(64, 1), FIndexed<int32>(36, 2), FIndexed<int32>(16, 3) });
}

#endif