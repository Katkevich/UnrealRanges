// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropOnEmptyRange, "UnrealRanges.Drop.TestDropEmptyRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropOnEmptyRange::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    auto Result = Ref(From).Drop(10);

    static_assert(std::is_same_v<TDropView<TRefView<TArray<int32>>, int32>, decltype(Result)>);

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropSkipAllElements, "UnrealRanges.Drop.TestDropSkipAllElements", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropSkipAllElements::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From).Drop(10);

    static_assert(std::is_same_v<TDropView<TRefView<TArray<int32>>, int32>, decltype(Result)>);

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropIsSizedIfUnderlyingViewIsSized, "UnrealRanges.Drop.TestDropIsSizedIfUnderlyingViewIsSized", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropIsSizedIfUnderlyingViewIsSized::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Num = Ref(From).Drop(3).Num();

    return Num == 2;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropIsNotSizedIfUnderlyingViewIsNotSized, "UnrealRanges.Drop.TestDropIsNotSizedIfUnderlyingViewIsNotSized", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropIsNotSizedIfUnderlyingViewIsNotSized::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5,6 };

    auto Result = Ref(From).Filter(Ur::Fn::Even).Drop(1);

    static_assert(!decltype(Result)::IsSized);

    return Result.Count() == 2 && EqualTo(Result, { 4,6 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_DropOofRValueItem, "UnrealRanges.Drop.TestDropOofRValueItem", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_DropOofRValueItem::RunTest(const FString& Parameters)
{
    const TCHAR* Chars = TEXT("abcdef");
    TArray<int32> From = { 1,2,3,4,5,6 };

    auto Result = Ref(From)
        .Transform([&](int32 Num) { return FString(Num, Chars); })
        .Drop(4);

    return EqualTo(Result, { TEXT("abcde"), TEXT("abcdef") });
}

#endif