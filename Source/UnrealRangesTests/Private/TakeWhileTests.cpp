// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeWhileEmptyRange, "UnrealRanges.TakeWhile.TestTakeWhileEmptyRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeWhileEmptyRange::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    auto Result = Ref(From).TakeWhile([](int32 Num) { return Num % 2 == 0; });

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeAll, "UnrealRanges.TakeWhile.TestTakeAll", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeAll::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 2,2,2 };

    const auto Result = Ref(From).TakeWhile([](int32 Num) { return Num % 2 == 0; });

    return EqualTo(Result, { 2,2,2 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeNothing, "UnrealRanges.TakeWhile.TestTakeNothing", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeNothing::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 2,2,2 };

    const auto Result = Ref(From).TakeWhile([](int32 Num) { return Num % 2 == 1; });

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeSome, "UnrealRanges.TakeWhile.TestTakeSome", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeSome::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 2,2,3 };

    const auto Result = Ref(From).TakeWhile([](int32 Num) { return Num % 2 == 0; });

    return EqualTo(Result, { 2,2 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeRValue, "UnrealRanges.TakeWhile.TestTakeRValue", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeRValue::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    const auto Result = Ref(From)
        .Transform([](const FString& Str) { return Str + Str; })
        .TakeWhile([](const FString& Str) { return Str.Len() == 2; });

    static_assert(std::is_same_v<decltype(*Result.begin()), FString>);

    return EqualTo(Result, { FString(TEXT("11")), FString(TEXT("22")) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeLValue, "UnrealRanges.TakeWhile.TestTakeLValue", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeLValue::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    const auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .TakeWhile([](const FString& Str) { return Str.Len() == 2; });

    static_assert(std::is_same_v<decltype(*Result.begin()), const FString&>);

    return EqualTo(Result, { FString(TEXT("1")), FString(TEXT("2")) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeLValueAfterReverse, "UnrealRanges.TakeWhile.TestTakeLValueAfterReverse", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeLValueAfterReverse::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    const auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .Reverse()
        .TakeWhile([](const FString& Str) { return Str.Len() == 2; });

    static_assert(std::is_same_v<decltype(*Result.begin()), const FString&>);

    return EqualTo(Result, { FString(TEXT("33")) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeInternalIteration, "UnrealRanges.TakeWhile.TestTakeInternalIteration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeInternalIteration::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .TakeWhile([](const FString& Str) { return Str.Len() == 2; })
        .To<TArray>();

    return EqualTo(Result, { FString(TEXT("1")), FString(TEXT("2")) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeInternalIterationEmptyResult, "UnrealRanges.TakeWhile.TestTakeInternalIterationEmptyResult", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeInternalIterationEmptyResult::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    auto Result = Ref(From)
        .TakeWhile([](const FString& Str) { return Str.Len() == 3; })
        .To<TArray>();

    return Result.Num() == 0;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeInternalIterationEmptySource, "UnrealRanges.TakeWhile.TestTakeInternalIterationEmptySource", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeInternalIterationEmptySource::RunTest(const FString& Parameters)
{
    TArray<FString> From;

    auto Result = Ref(From)
        .TakeWhile([](const FString& Str) { return Str.Len() == 3; })
        .To<TArray>();

    return Result.Num() == 0;
}

//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeWhileOnDifferentViews, "UnrealRanges.TakeWhile.TestTakeWhileOnDifferentViews", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
//bool FUnrealRangesTests_TakeWhileOnDifferentViews::RunTest(const FString& Parameters)
//{
//    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };
//
//    bool bResult = true;
//    {
//        auto Result = Ref(From)
//            .TakeWhile([](const FString& Str) { return Str.Len() == 1; });
//
//        bResult &= std::distance(Result.begin(), Result.end()) == 2;
//    }
//    {
//        auto Result = Ref(From)
//            .Filter([](const FString& Str) { return Str.Len() == 1; })
//            .TakeWhile([](const FString& Str) { return Str.Len() == 3; });
//
//        bResult &= std::distance(Result.begin(), Result.end()) == 2;
//    }
//
//}

#endif