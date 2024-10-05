// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"
#include <forward_list>

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewNotEqualToEmptyNonSizedRange, "UnrealRanges.EqualTo.TestNonEmptyViewNotEqualToEmptyNonSizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewNotEqualToEmptyNonSizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2,3 };
    std::forward_list<int32> That;

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewNotEqualToEmptySizedRange, "UnrealRanges.EqualTo.TestNonEmptyViewNotEqualToEmptySizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewNotEqualToEmptySizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2,3 };
    TArray<int32> That;

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EmptyViewEqualToEmptyNonSizedRange, "UnrealRanges.EqualTo.TestEmptyViewEqualToEmptyNonSizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EmptyViewEqualToEmptyNonSizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz;
    std::forward_list<int32> That;

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EmptyViewEqualToEmptySizedRange, "UnrealRanges.EqualTo.TestEmptyViewEqualToEmptySizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EmptyViewEqualToEmptySizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz;
    TArray<int32> That;

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return bEqualTo;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewEqualToNonEmptyNonSizedRangeWithEqSize, "UnrealRanges.EqualTo.TestNonEmptyViewEqualToNonEmptyNonSizedRangeWithEqSize", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewEqualToNonEmptyNonSizedRangeWithEqSize::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2,3 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewEqualToNonEmptySizedRangeWithEqSize, "UnrealRanges.EqualTo.TestNonEmptyViewEqualToNonEmptySizedRangeWithEqSize", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewEqualToNonEmptySizedRangeWithEqSize::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2,3 };
    TArray<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewEqualToNonEmptySizedRangeWithEqSizeWithDiffItemType, "UnrealRanges.EqualTo.TestNonEmptyViewEqualToNonEmptySizedRangeWithEqSizeWithDiffItemType", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewEqualToNonEmptySizedRangeWithEqSizeWithDiffItemType::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2,3 };
    TArray<float> That = { 1.f,2.f,3.f };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EmptyViewNotEqualToNonEmptyNonSizedRange, "UnrealRanges.EqualTo.TestEmptyViewNotEqualToNonEmptyNonSizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EmptyViewNotEqualToNonEmptyNonSizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz;
    std::forward_list<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EmptyViewNotEqualToNonEmptySizedRange, "UnrealRanges.EqualTo.TestEmptyViewNotEqualToNonEmptySizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EmptyViewNotEqualToNonEmptySizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz;
    TArray<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewNotEqualToNonEmptyNonSizedRange, "UnrealRanges.EqualTo.TestNonEmptyViewNotEqualToNonEmptyNonSizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewNotEqualToNonEmptyNonSizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewNotEqualToNonEmptySizedRange, "UnrealRanges.EqualTo.TestNonEmptyViewNotEqualToNonEmptySizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewNotEqualToNonEmptySizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2 };
    TArray<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewNotEqualToNonEmptyNonSizedRangeSameSizeDiffValues, "UnrealRanges.EqualTo.TestNonEmptyViewNotEqualToNonEmptyNonSizedRangeSameSizeDiffValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewNotEqualToNonEmptyNonSizedRangeSameSizeDiffValues::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,3,4 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewNotEqualToNonEmptySizedRangeSameSizeDiffValues, "UnrealRanges.EqualTo.TestNonEmptyViewNotEqualToNonEmptySizedRangeSameSizeDiffValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewNotEqualToNonEmptySizedRangeSameSizeDiffValues::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,3,4 };
    TArray<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

#endif