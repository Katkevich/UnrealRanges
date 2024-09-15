// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Fn/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_SumEmptyIntRange, "UnrealRanges.Sum.TestSumEmptyIntRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_SumEmptyIntRange::RunTest(const FString& Parameters)
{
    TArray<int32> Array;

    return Ref(Array).Sum() == 0;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_SumEmptyStringRange, "UnrealRanges.Sum.TestSumEmptyStringRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_SumEmptyStringRange::RunTest(const FString& Parameters)
{
    TArray<FString> Array;

    return Ref(Array).Sum() == FString();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_SumEmptyStringRangeUsingProjection, "UnrealRanges.Sum.TestSumEmptyStringRangeUsingProjection", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_SumEmptyStringRangeUsingProjection::RunTest(const FString& Parameters)
{
    TArray<FString> Array;

    return Ref(Array).Sum(&FString::Len) == 0;
}

struct FItem
{
    int32 Value{};

private:
    friend int32 operator+(const FItem& Lhs, const FItem& Rhs)
    {
        return Lhs.Value + Rhs.Value;
    }

    friend int32 operator+(int32 Lhs, const FItem& Rhs)
    {
        return Lhs + Rhs.Value;
    }
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_SumRangeWithCustomItem, "UnrealRanges.Sum.TestSumRangeWithCustomItem", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_SumRangeWithCustomItem::RunTest(const FString& Parameters)
{
    TArray<FItem> Array = { {1}, {2}, {3} };

    return Ref(Array).Sum() == 6;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_SumOverRValues, "UnrealRanges.Sum.TestSumOverRValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_SumOverRValues::RunTest(const FString& Parameters)
{
    const TCHAR* Chars = TEXT("abcdef");
    TArray<int32> Array = { 1,2,3 };

    auto Result = Ref(Array)
        .Transform([&](int32 Num) { return FString(Num, Chars); })
        .Sum();

    return Result == FString(TEXT("aababc"));
}

#endif