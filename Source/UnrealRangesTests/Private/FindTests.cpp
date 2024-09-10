// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FindEmptyRange, "UnrealRanges.Find.TestFindEmptyRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FindEmptyRange::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    auto Result = Ref(From).Find(1);

    static_assert(std::is_same_v<TOptional<int32&>, decltype(Result)>);

    return !Result.IsSet();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FindWithImplicitConvertsion, "UnrealRanges.Find.TestFindWithImplicitConvertsion", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FindWithImplicitConvertsion::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("aa")) , FString(TEXT("aaa")), FString(TEXT("aaa")) };

    auto Result = Ref(From).Find(FStringView(TEXT("aa")));

    static_assert(std::is_same_v<TOptional<FString&>, decltype(Result)>);

    return Result.IsSet() && *Result == FString(TEXT("aa"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FindReturnsVeryFirstMatch, "UnrealRanges.Find.TestFindReturnsVeryFirstMatch", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FindReturnsVeryFirstMatch::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("aa")) , FString(TEXT("aa")), FString(TEXT("aa")) };

    TOptional<FString&> Result = Ref(From).Find(FStringView(TEXT("aa")));

    return Result.GetPtrOrNull() == std::addressof(From[1]);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FindWithProjectionWhichReturnsRValue, "UnrealRanges.Find.TestFindWithProjectionWhichReturnsRValue", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FindWithProjectionWhichReturnsRValue::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("a")), FString(TEXT("b")) };

    TOptional<FString&> Result = Ref(From).Find(TEXT("bb"), [](const FString& Str) { return Str + Str; });

    return Result.GetPtrOrNull() == std::addressof(From[2]);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FindWithProjectionBeingMemberPtr, "UnrealRanges.Find.TestFindWithProjectionBeingMemberPtr", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FindWithProjectionBeingMemberPtr::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("aa")), FString(TEXT("b")), FString(TEXT("b")) };

    TOptional<FString&> Result = Ref(From).Find(2, &FString::Len);

    return Result.GetPtrOrNull() == std::addressof(From[1]);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FindByPredicate, "UnrealRanges.Find.TestFindByPredicate", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FindByPredicate::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("23")), FString(TEXT("233")), FString(TEXT("2333")) };

    TOptional<FString&> Result = Ref(From).FindBy([](FString& Str) { return Str == TEXT("23"); });

    return Result.GetPtrOrNull() == std::addressof(From[1]);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FindCalledOnConstRangeReturnsConstResult, "UnrealRanges.Find.TestFindCalledOnConstRangeReturnsConstResult", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FindCalledOnConstRangeReturnsConstResult::RunTest(const FString& Parameters)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("23")), FString(TEXT("233")), FString(TEXT("2335")), FString(TEXT("2334")) };
    const TRefView<TArray<FString>> RefView = Ref(From);

    TOptional<const FString&> Result0 = RefView.Find(TEXT("233"));
    TOptional<const FString&> Result1 = RefView.Find(3, &FString::Len);
    TOptional<const FString&> Result2 = RefView.FindBy([](const FString& Str) { return Str == TEXT("23"); });

    return Result0.GetPtrOrNull() == std::addressof(From[2]) && Result1.GetPtrOrNull() == std::addressof(From[2]) && Result2.GetPtrOrNull() == std::addressof(From[1]);
}

#endif