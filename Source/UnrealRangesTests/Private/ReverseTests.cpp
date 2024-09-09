// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ReverseEmpty, "UnrealRanges.MinMax.TestReverseEmpty", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ReverseEmpty::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    auto Result = Ref(From).Reverse();

    static_assert(std::is_same_v<TReverseView<TRefView<TArray<int32>>>, decltype(Result)>);

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ReverseNonEmpty, "UnrealRanges.MinMax.TestReverseNonEmpty", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ReverseNonEmpty::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From).Reverse();

    return EqualTo(Result, { 5,4,3,2,1 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DoubleReverseChangeNothing, "UnrealRanges.MinMax.TestDoubleReverseChangeNothing", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DoubleReverseChangeNothing::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From).Reverse().Reverse();

    return EqualTo(Result, { 1,2,3,4,5 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DoubleReverseWithTransformInTheMiddleChangeNothing, "UnrealRanges.MinMax.TestDoubleReverseWithTransformInTheMiddleChangeNothing", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DoubleReverseWithTransformInTheMiddleChangeNothing::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From)
        .Reverse()
        .Transform([](int32 Num) { return Num * Num; })
        .Reverse();

    return EqualTo(Result, { 1,4,9,16,25 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ReverseAfterFilter, "UnrealRanges.MinMax.TestReverseAfterFilter", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ReverseAfterFilter::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From)
        .Filter([](int32 Num) { return Num % 2 == 1; })
        .Reverse();

    return EqualTo(Result, { 5, 3, 1 });
}

#endif