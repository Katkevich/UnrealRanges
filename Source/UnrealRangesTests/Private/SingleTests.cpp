// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Traits.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

struct FMoveOnly
{
    FMoveOnly() = default;
    FMoveOnly(FMoveOnly&&) = default;

    FMoveOnly(const FMoveOnly&) = delete;
    FMoveOnly& operator=(const FMoveOnly&) = delete;
    FMoveOnly& operator=(FMoveOnly&&) = delete;

    int Num = 0;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_SingleMakeValue, "UnrealRanges.Single.MakeValue", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_SingleMakeValue::RunTest(const FString& Parameters)
{
    // Values
    {
        decltype(auto) Result = Ur::View::Single(FString());
        static_assert(std::is_same_v<decltype(Result), TSingleView<FString>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FString&>);
    }
    {
        FString Str;
        decltype(auto) Result = Ur::View::Single(Str);
        static_assert(std::is_same_v<decltype(Result), TSingleView<FString>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FString&>);
    }
    {
        const FString Str;
        decltype(auto) Result = Ur::View::Single(Str);
        static_assert(std::is_same_v<decltype(Result), TSingleView<FString>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FString&>);
    }
    {
        FMoveOnly Str;
        decltype(auto) Result = Ur::View::Single(MoveTemp(Str));
        static_assert(std::is_same_v<decltype(Result), TSingleView<FMoveOnly>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FMoveOnly&>);
    }
    // References

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_SingleMixWithConcat, "UnrealRanges.Single.MixWithConcat", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_SingleMixWithConcat::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { TEXT("1"), TEXT("abba1") , TEXT("abba2"), TEXT("2") };

    auto Result = Ref(Array)
        .Filter([](const FString& Str) { return Str.Len() > 1; })
        .ConcatWith(Single(FString(TEXT("single1"))), Single(FString(TEXT("single2"))))
        .Transform([](FString& Str) { return Str.RightChop(1); });

    return EqualTo(Result, { TEXT("bba1") , TEXT("bba2"), TEXT("ingle1"), TEXT("ingle2") });
}

#endif