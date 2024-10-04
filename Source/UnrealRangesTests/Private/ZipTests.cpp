// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ZipSingleEmptyRange, "UnrealRanges.Zip.TestZipSingleEmptyRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ZipSingleEmptyRange::RunTest(const FString& Parameters)
{
    TArray<int32> From1 = { };

    auto Result = Zip(From1);

    static_assert(std::is_same_v<TTuple<int32&>, decltype(*Result.begin())>);

    return EqualTo(Result, std::initializer_list<TTuple<int32&>>{ });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ZipSingleNonEmptyRange, "UnrealRanges.Zip.TestZipSingleNonEmptyRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ZipSingleNonEmptyRange::RunTest(const FString& Parameters)
{
    TArray<int32> From1 = { 1,2,3 };

    auto Result = Zip(From1);

    static_assert(std::is_same_v<TTuple<int32&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0]), ForwardAsTuple(From1[1]), ForwardAsTuple(From1[2]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ZipTwoRangesSecondIsShorter, "UnrealRanges.Zip.TestZipTwoRangesSecondIsShorter", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ZipTwoRangesSecondIsShorter::RunTest(const FString& Parameters)
{
    TArray<int32> From1 = { 1,2 };
    TArray<FString> From2 = { TEXT("1"), TEXT("2"), TEXT("3")};

    auto Result = Zip(From1, From2);

    static_assert(std::is_same_v<TTuple<int32&, FString&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0]), ForwardAsTuple(From1[1], From2[1]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ZipTwoRangesFirstIsShorter, "UnrealRanges.Zip.TestZipTwoRangesFirstIsShorter", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ZipTwoRangesFirstIsShorter::RunTest(const FString& Parameters)
{
    TArray<int32> From1 = { 1,2,3 };
    TArray<FString> From2 = { TEXT("1"), TEXT("2") };

    auto Result = Zip(From1, From2);

    static_assert(std::is_same_v<TTuple<int32&, FString&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0]), ForwardAsTuple(From1[1], From2[1]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ZipTwoRangesSameLength, "UnrealRanges.Zip.TestZipTwoRangesSameLength", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ZipTwoRangesSameLength::RunTest(const FString& Parameters)
{
    TArray<int32> From1 = { 1,2 };
    TArray<FString> From2 = { TEXT("1"), TEXT("2") };

    auto Result = Zip(From1, From2);

    static_assert(std::is_same_v<TTuple<int32&, FString&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0]), ForwardAsTuple(From1[1], From2[1]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ZipWithTwoRangesSelfIsShorter, "UnrealRanges.Zip.TestZipWithTwoRangesSelfIsShorter", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ZipWithTwoRangesSelfIsShorter::RunTest(const FString& Parameters)
{
    TArray<double> From1 = { 10.0, 20.0 };
    TArray<int32> From2 = { 1,2,3 };
    TArray<FString> From3 = { TEXT("1"), TEXT("2"), TEXT("3")};

    auto Result = Ref(From1)
        .ZipWith(From2, From3);

    static_assert(std::is_same_v<TTuple<double&, int32&, FString&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0], From3[0]), ForwardAsTuple(From1[1], From2[1], From3[1]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ZipWithTwoRangesSelfIsLong, "UnrealRanges.Zip.TestZipWithTwoRangesSelfIsLonger", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ZipWithTwoRangesSelfIsLong::RunTest(const FString& Parameters)
{
    TArray<double> From1 = { 10.0, 20.0, 30.0 };
    TArray<int32> From2 = { 1,2 };
    TArray<FString> From3 = { TEXT("1"), TEXT("2") };

    auto Result = Ref(From1)
        .ZipWith(From2, From3);

    static_assert(std::is_same_v<TTuple<double&, int32&, FString&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0], From3[0]), ForwardAsTuple(From1[1], From2[1], From3[1]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ZipInternalIterationSelfIsLonger, "UnrealRanges.Zip.TestZipInternalIterationSelfIsLonger", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ZipInternalIterationSelfIsLonger::RunTest(const FString& Parameters)
{
    TArray<double> From1 = { 10.0, 20.0, 30.0 };
    TArray<int32> From2 = { 1,2 };
    TArray<FString> From3 = { TEXT("1"), TEXT("2") };

    auto Result = Ref(From1)
        .ZipWith(From2, From3)
        .To<TArray>();

    static_assert(std::is_same_v<TTuple<double&, int32&, FString&>&, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0], From3[0]), ForwardAsTuple(From1[1], From2[1], From3[1]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ZipInternalIterationSelfIsShorter, "UnrealRanges.Zip.TestZipInternalIterationSelfIsShorter", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ZipInternalIterationSelfIsShorter::RunTest(const FString& Parameters)
{
    TArray<double> From1 = { 10.0, 20.0 };
    TArray<int32> From2 = { 1,2,3 };
    TArray<FString> From3 = { TEXT("1"), TEXT("2") };

    auto Result = Ref(From1)
        .ZipWith(From2, From3)
        .To<TArray>();

    static_assert(std::is_same_v<TTuple<double&, int32&, FString&>&, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0], From3[0]), ForwardAsTuple(From1[1], From2[1], From3[1]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ZipSized, "UnrealRanges.Zip.TestZipSized", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ZipSized::RunTest(const FString& Parameters)
{
    TArray<double> From1 = { 10.0, 20.0 };
    TArray<int32> From2 = { 1,2,3 };
    TArray<FString> From3 = { TEXT("1"), TEXT("2") };

    auto Result = Ref(From1)
        .ZipWith(From2, From3);

    static_assert(std::is_same_v<TTuple<double&, int32&, FString&>, decltype(*Result.begin())>);

    return Result.Num() == 2;
}

#endif