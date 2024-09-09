// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "Containers/Array.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MinOnEmptyRangeReturnsEmptyOptional, "UnrealRanges.MinMax.TestMinOnEmptyRangeReturnsEmptyOptional", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MinOnEmptyRangeReturnsEmptyOptional::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    TOptional<int32&> Result = Ur::View::Ref(From).Min();

    return !Result.IsSet();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaxOnEmptyRangeReturnsEmptyOptional, "UnrealRanges.MinMax.TestMaxOnEmptyRangeReturnsEmptyOptional", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaxOnEmptyRangeReturnsEmptyOptional::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    TOptional<int32&> Result = Ur::View::Ref(From).Max();

    return !Result.IsSet();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MinMaxOnEmptyRangeReturnsEmptyOptional, "UnrealRanges.MinMax.TestMinMaxOnEmptyRangeReturnsEmptyOptional", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MinMaxOnEmptyRangeReturnsEmptyOptional::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    Ur::TMinMax<int32&> Result = Ur::View::Ref(From).MinMax();

    return !Result.Min.IsSet() && !Result.Max.IsSet();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MinReturnsReferenceFstMin, "UnrealRanges.MinMax.TestMinReturnsReferenceFstMin", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MinReturnsReferenceFstMin::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1, 2, 3 };

    TOptional<int32&> Result = Ur::View::Ref(From).Min();

    return Result.IsSet() && Result.GetValue() == 1;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MinReturnsReferenceLstMin, "UnrealRanges.MinMax.TestMinReturnsReferenceLstMin", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MinReturnsReferenceLstMin::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 2, 3, 1 };

    TOptional<int32&> Result = Ur::View::Ref(From).Min();

    return Result.IsSet() && Result.GetValue() == 1;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaxReturnsReferenceFstMax, "UnrealRanges.MinMax.TestMaxReturnsReferenceFstMax", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaxReturnsReferenceFstMax::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 3, 2, 1 };

    TOptional<int32&> Result = Ur::View::Ref(From).Max();

    return Result.IsSet() && Result.GetValue() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaxReturnsReferenceLstMax, "UnrealRanges.MinMax.TestMaxReturnsReferenceLstMax", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaxReturnsReferenceLstMax::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1, 2, 3 };

    TOptional<int32&> Result = Ur::View::Ref(From).Max();

    return Result.IsSet() && Result.GetValue() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MinMaxReturnsReferenceSingleElement, "UnrealRanges.MinMax.TestMinMaxReturnsReferenceSingleElement", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MinMaxReturnsReferenceSingleElement::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1 };

    Ur::TMinMax<int32&> Result = Ur::View::Ref(From).MinMax();

    return Result.Min.IsSet() && Result.Max.IsSet() && Result.Min.GetValue() == 1 && Result.Max.GetValue() == 1;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MinMaxReturnsReferenceDiffElements, "UnrealRanges.MinMax.TestMinMaxReturnsReferenceDiffElements", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MinMaxReturnsReferenceDiffElements::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3 };

    Ur::TMinMax<int32&> Result = Ur::View::Ref(From).MinMax();

    return Result.Min.IsSet() && Result.Max.IsSet() && Result.Min.GetValue() == 1 && Result.Max.GetValue() == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MinMaxReturnsReferenceSeveralDuplicates, "UnrealRanges.MinMax.TestMinMaxReturnsReferenceSeveralDuplicates", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MinMaxReturnsReferenceSeveralDuplicates::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,1,1 };

    Ur::TMinMax<int32&> Result = Ur::View::Ref(From).MinMax();

    return Result.Min.IsSet() && Result.Max.IsSet() && Result.Min.GetValue() == 1 && Result.Max.GetValue() == 1 && Result.Min.GetPtrOrNull() == Result.Max.GetPtrOrNull();
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MinBy, "UnrealRanges.MinMax.TestMinBy", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MinBy::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("aa")), FString(TEXT("aaa")) , FString(TEXT("a")) };

    auto Result = Ur::View::Ref(From)
        .Min([](FString& Str) { return Str.Len(); });

    static_assert(std::is_same_v<TOptional<FString&>, decltype(Result)>, "err");

    return Result.GetPtrOrNull() == std::addressof(From[2]);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaxBy, "UnrealRanges.MinMax.TestMaxBy", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaxBy::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("aa")), FString(TEXT("aaa")) , FString(TEXT("a")) };

    auto Result = Ur::View::Ref(From)
        .Max([](FString& Str) { return Str.Len(); });

    static_assert(std::is_same_v<TOptional<FString&>, decltype(Result)>, "err");

    return Result.GetPtrOrNull() == std::addressof(From[1]);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaxByMemberPtrProjection, "UnrealRanges.MinMax.TestMaxByMemberPtrProjection", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaxByMemberPtrProjection::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("aa")), FString(TEXT("aaa")) , FString(TEXT("a")) };

    auto Result = Ur::View::Ref(From)
        .Max(&FString::Len);

    static_assert(std::is_same_v<TOptional<FString&>, decltype(Result)>, "err");

    return Result.GetPtrOrNull() == std::addressof(From[1]);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MinMaxBy, "UnrealRanges.MinMax.TestMinMaxBy", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MinMaxBy::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("aa")), FString(TEXT("aaa")) , FString(TEXT("a")) };

    auto Result = Ur::View::Ref(From)
        .MinMax([](FString& Str) { return Str.Len(); });

    static_assert(std::is_same_v<Ur::TMinMax<FString&>, decltype(Result)>, "err");

    return Result.Min.GetPtrOrNull() == std::addressof(From[2]) && Result.Max.GetPtrOrNull() == std::addressof(From[1]);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaxRValueResult, "UnrealRanges.MinMax.TestMaxRValueResult", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaxRValueResult::RunTest(const FString& Parameters)
{
    const TCHAR* Str = TEXT("12345");
    TArray<int32> Lens = { 1,3,2,1 };

    auto Result = Ur::View::Ref(Lens)
        .Transform([&](int32 Len) { return FString(Len, Str); })
        .Max([](const FString& Str) { return Str.Len(); });

    static_assert(std::is_same_v<TOptional<FString>, decltype(Result)>, "err");

    return Result.IsSet() && *Result == TEXT("123");
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_OperatorLessIsRequired, "UnrealRanges.MinMax.TestOperatorLessIsRequired", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_OperatorLessIsRequired::RunTest(const FString& Parameters)
{
    struct Foo
    {
        auto operator<(const Foo&) const = delete;
    };
    TArray<Foo> Foos;

    //Ur::View::Ref(Foos).Max(); // doesn't compile!

    return true;
}

#endif