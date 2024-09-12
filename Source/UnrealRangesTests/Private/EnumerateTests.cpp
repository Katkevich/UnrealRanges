// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EnumerateEmptyRange, "UnrealRanges.Filter.TestEnumerateEmptyRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EnumerateEmptyRange::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    auto Result = Ref(From).Enumerate();

    static_assert(std::is_same_v<TEnumerateView<TRefView<TArray<int32>>, int32>, decltype(Result)>);

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EnumerateLValues, "UnrealRanges.Filter.TestEnumerateLValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EnumerateLValues::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    auto Result = Ref(From).Enumerate();

    static_assert(std::is_same_v<decltype(*Result.begin()), FIndexed<FString&>>);

    for (FIndexed<FString&> Value : Result)
    {
        (void)Value;
    }

    return EqualTo(Result, { FIndexed<FString&>(From[0], 0), FIndexed<FString&>(From[1], 1), FIndexed<FString&>(From[2], 2)});
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EnumerateRValues, "UnrealRanges.Filter.TestEnumerateRValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EnumerateRValues::RunTest(const FString& Parameters)
{
    const TCHAR* Chars = TEXT("12345");

    TArray<int32> From = { 1,2,3 };

    auto Result = Ref(From)
        .Transform([&](int32 Num) { return FString(Num, Chars); })
        .Enumerate();

    static_assert(std::is_same_v<decltype(*Result.begin()), FIndexed<FString>>);

    for (FIndexed<FString> Value : Result)
    {
        (void)Value;
    }

    return EqualTo(Result, { FIndexed<FString>(TEXT("1"), 0), FIndexed<FString>(TEXT("12"), 1), FIndexed<FString>(TEXT("123"), 2) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EnumerateRValuesWithDifferentDefaults, "UnrealRanges.Filter.TestEnumerateRValuesWithDifferentDefaults", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EnumerateRValuesWithDifferentDefaults::RunTest(const FString& Parameters)
{
    const TCHAR* Chars = TEXT("12345");

    TArray<int32> From = { 1,2,3 };

    auto Result = Ref(From)
        .Transform([&](int32 Num) { return FString(Num, Chars); })
        .Reverse()
        .Enumerate(1, 2);

    static_assert(std::is_same_v<decltype(*Result.begin()), FIndexed<FString>>);

    for (FIndexed<FString> Value : Result)
    {
        (void)Value;
    }

    return EqualTo(Result, { FIndexed<FString>(TEXT("123"), 1), FIndexed<FString>(TEXT("12"), 3), FIndexed<FString>(TEXT("1"), 5) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EnumeratedInternalIteration, "UnrealRanges.Filter.TestEnumeratedInternalIteration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EnumeratedInternalIteration::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3 };

    auto Result = Enumerate(From, 2, 3)
        .Filter([&](FIndexed<int32&> Num) { return Num.Index % 2 == 0; })
        .To<TArray>();

    return EqualTo(Result, { FIndexed<int32&>(From[0], 2), FIndexed<int32&>(From[2], 8) });
}


#endif