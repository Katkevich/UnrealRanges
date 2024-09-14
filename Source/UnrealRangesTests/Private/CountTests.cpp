// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_RefViewCount, "UnrealRanges.Count.TestRefView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_RefViewCount::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Count() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EnumerateViewCount, "UnrealRanges.Count.TestEnumerateView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EnumerateViewCount::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Enumerate().Count() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TransformViewCount, "UnrealRanges.Count.TestTransformVIew", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TransformViewCount::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Transform([](int32 Num) { return Num; }).Count() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ReverseViewCount, "UnrealRanges.Count.TestReverseView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ReverseViewCount::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Reverse().Count() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeViewCountGreaterThanArray, "UnrealRanges.Count.TestTakeViewGreaterThanArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeViewCountGreaterThanArray::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(10).Count() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeViewCountLessThanArray, "UnrealRanges.Count.TestTakeViewLessThanArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeViewCountLessThanArray::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(2).Count() == 2;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FilterViewCount, "UnrealRanges.Count.TestFilterView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FilterViewCount::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Filter([](int32 Num) { return Num % 2 == 1; }).Count() == 2;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeWhileViewCount, "UnrealRanges.Count.TestTakeWhileView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeWhileViewCount::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).TakeWhile([](int32 Num) { return Num <= 2; }).Count() == 2;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeWhilePlusEnumerate, "UnrealRanges.Count.TestTakeWhilePlusEnumerateView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeWhilePlusEnumerate::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).TakeWhile([](int32 Num) { return Num <= 2; }).Enumerate().Count() == 2;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_CountIf, "UnrealRanges.CountIf.TestCountIf", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_CountIf::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3,4,5,6,7 };

    return Ref(Array).CountIf([](int32 Num) { return Num % 2 == 0; }) == 3;
}
#endif