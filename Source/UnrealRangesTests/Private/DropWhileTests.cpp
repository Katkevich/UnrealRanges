// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropWhileEmptyRange, "UnrealRanges.DropWhile.TestDropWhileEmptyRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropWhileEmptyRange::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    auto Result = Ref(From).DropWhile([](int32 Num) { return Num % 2 == 0; });

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropAll, "UnrealRanges.DropWhile.TestDropAll", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropAll::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 2,2,2 };

    const auto Result = Ref(From).DropWhile([](int32 Num) { return Num % 2 == 0; });

    return EqualTo(Result, std::initializer_list<int32>{});
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropNothing, "UnrealRanges.DropWhile.TestDropNothing", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropNothing::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 2,2,2 };

    const auto Result = Ref(From).DropWhile([](int32 Num) { return Num % 2 == 1; });

    return EqualTo(Result, { 2,2,2 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropSome, "UnrealRanges.DropWhile.TestDropSome", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropSome::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 2,2,3 };

    const auto Result = Ref(From).DropWhile([](int32 Num) { return Num % 2 == 0; });

    return EqualTo(Result, { 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropWhileRValue, "UnrealRanges.DropWhile.TestDropRValue", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropWhileRValue::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    const auto Result = Ref(From)
        .Transform([](const FString& Str) { return Str + Str; })
        .DropWhile([](const FString& Str) { return Str.Len() == 2; });

    static_assert(std::is_same_v<decltype(*Result.begin()), FString>);

    return EqualTo(Result, { FString(TEXT("3333")) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropWhileLValue, "UnrealRanges.DropWhile.TestDropLValue", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropWhileLValue::RunTest(const FString& Parameters)
{
    TArray<FString> From = { TEXT("1"), TEXT("2"), TEXT("3"), TEXT("33") };

    const auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .DropWhile([](const FString& Str) { return Str == TEXT("1"); });

    static_assert(std::is_same_v<decltype(*Result.begin()), const FString&>);

    return EqualTo(Result, { TEXT("2"), TEXT("3") });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropWhileLValueAfterReverse, "UnrealRanges.DropWhile.TestDropLValueAfterReverse", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropWhileLValueAfterReverse::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")), FString(TEXT("33")) };

    const auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .Reverse()
        .DropWhile([](const FString& Str) { return Str == TEXT("3"); });

    static_assert(std::is_same_v<decltype(*Result.begin()), const FString&>);

    return EqualTo(Result, { TEXT("2"), TEXT("1") });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropWhileInternalIteration, "UnrealRanges.DropWhile.TestDropWhileInternalIteration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropWhileInternalIteration::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .DropWhile([](const FString& Str) { return Str == TEXT("1"); })
        .To<TArray>();

    return EqualTo(Result, { TEXT("2") });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropWhileNothingToDrop, "UnrealRanges.DropWhile.TestDropWhileNothingToDrop", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropWhileNothingToDrop::RunTest(const FString& Parameters)
{
    TArray<FString> From = { TEXT("1"), TEXT("2"), TEXT("33") };

    auto Result = Ref(From)
        .DropWhile([](const FString& Str) { return Str.Len() == 3; })
        .To<TArray>();

    return EqualTo(Result, { TEXT("1"), TEXT("2"), TEXT("33") });
}

#endif