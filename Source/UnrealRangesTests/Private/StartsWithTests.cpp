// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"
#include <forward_list>

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewStartsWithEmptyNonSizedRange, "UnrealRanges.StartsWith.TestNonEmptyViewStartsWithEmptyNonSizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewStartsWithEmptyNonSizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2,3 };
    std::forward_list<int32> That;

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewStartsWithEmptySizedRange, "UnrealRanges.StartsWith.TestNonEmptyViewStartsWithEmptySizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewStartsWithEmptySizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2,3 };
    TArray<int32> That;

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EmptyViewStartsWithEmptyNonSizedRange, "UnrealRanges.StartsWith.TestEmptyViewStartsWithEmptyNonSizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EmptyViewStartsWithEmptyNonSizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz;
    std::forward_list<int32> That;

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EmptyViewStartsWithEmptySizedRange, "UnrealRanges.StartsWith.TestEmptyViewStartsWithEmptySizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EmptyViewStartsWithEmptySizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz;
    TArray<int32> That;

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewStartsWithNonEmptyNonSizedRangeWithEqSize, "UnrealRanges.StartsWith.TestNonEmptyViewStartsWithNonEmptyNonSizedRangeWithEqSize", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewStartsWithNonEmptyNonSizedRangeWithEqSize::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2,3 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewStartsWithNonEmptySizedRangeWithEqSize, "UnrealRanges.StartsWith.TestNonEmptyViewStartsWithNonEmptySizedRangeWithEqSize", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewStartsWithNonEmptySizedRangeWithEqSize::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2,3 };
    TArray<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EmptyViewStartsWithNonEmptyNonSizedRange, "UnrealRanges.StartsWith.TestEmptyViewStartsWithNonEmptyNonSizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EmptyViewStartsWithNonEmptyNonSizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz;
    std::forward_list<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EmptyViewIsntStartWithNonEmptySizedRange, "UnrealRanges.StartsWith.TestEmptyViewIsntStartWithNonEmptySizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EmptyViewIsntStartWithNonEmptySizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz;
    TArray<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewIsntStartWithNonEmptyNonSizedRange, "UnrealRanges.StartsWith.TestNonEmptyViewIsntStartWithNonEmptyNonSizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewIsntStartWithNonEmptyNonSizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewIsntStartWithNonEmptySizedRange, "UnrealRanges.StartsWith.TestNonEmptyViewIsntStartWithNonEmptySizedRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewIsntStartWithNonEmptySizedRange::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,2 };
    TArray<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewIsntStartWithNonEmptyNonSizedRangeSameSizeDiffValues, "UnrealRanges.StartsWith.TestNonEmptyViewIsntStartWithNonEmptyNonSizedRangeSameSizeDiffValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewIsntStartWithNonEmptyNonSizedRangeSameSizeDiffValues::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,3,4 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_NonEmptyViewIsntStartWithNonEmptySizedRangeSameSizeDiffValues, "UnrealRanges.StartsWith.TestNonEmptyViewIsntStartWithNonEmptySizedRangeSameSizeDiffValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_NonEmptyViewIsntStartWithNonEmptySizedRangeSameSizeDiffValues::RunTest(const FString& Parameters)
{
    TArray<int32> Thiz = { 1,3,4 };
    TArray<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

#endif