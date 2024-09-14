// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur;
using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_RefViewSize, "UnrealRanges.Size.TestRefView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_RefViewSize::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Num() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EnumerateViewSize, "UnrealRanges.Size.TestEnumerateView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EnumerateViewSize::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Enumerate().Num() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TransformViewSize, "UnrealRanges.Size.TestTransformVIew", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TransformViewSize::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Transform([](int32 Num) { return Num; }).Num() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ReverseViewSize, "UnrealRanges.Size.TestReverseView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ReverseViewSize::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Reverse().Num() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeViewSizeGreaterThanArray, "UnrealRanges.Size.TestTakeViewGreaterThanArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeViewSizeGreaterThanArray::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(10).Num() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeViewSizeLessThanArray, "UnrealRanges.Size.TestTakeViewLessThanArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeViewSizeLessThanArray::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(2).Num() == 2;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_SizedRangeConcept, "UnrealRanges.Size.TestSizedRangeConcept", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_SizedRangeConcept::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };
    FString Str;

    static_assert(SizedRange<TArray<int32>>);
    static_assert(SizedRange<decltype(Ref(Array))>);
    static_assert(SizedRange<decltype(Ref(Array).Reverse())>);
    static_assert(SizedRange<decltype(Ref(Array).Enumerate())>);
    static_assert(SizedRange<decltype(Ref(Array).Transform([](int32 Num) { return Num; }))>);
    static_assert(SizedRange<decltype(Ref(Array).Take(5))>);
    static_assert(SizedRange<decltype(Ref(Str))>);
    static_assert(SizedRange<decltype(Ref(Str).Reverse())>);
    static_assert(SizedRange<decltype(Ref(Str).Enumerate())>);
    static_assert(SizedRange<decltype(Ref(Str).Take(5))>);

    static_assert(!SizedRange<decltype(Ref(Array).TakeWhile([](int32 Num) { return Num > 0; }))>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }))>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }).Enumerate())>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }).Reverse())>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }).Take(5))>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }).Transform([](int32 Num) { return Num; }))>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }).Reverse().Take(5))>);

    return true;
}

#endif