// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Traits.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaybeFactoryCallResultType, "UnrealRanges.Maybe.FactoryCallResultType", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaybeFactoryCallResultType::RunTest(const FString& Parameters)
{
    // Values
    {
        decltype(auto) Result = Ur::View::MaybeVal(FString());
        static_assert(std::is_same_v<decltype(Result), TMaybeView<FString>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FString&>);
    }
    {
        FString Str;
        decltype(auto) Result = Ur::View::MaybeVal(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<FString>>);
    }
    {
        const FString Str;
        decltype(auto) Result = Ur::View::MaybeVal(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<FString>>);
    }
    {
        TOptional<FString> Str;
        decltype(auto) Result = Ur::View::MaybeVal(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<TOptional<FString>>>);
    }
    {
        TOptional<const FString> Str;
        decltype(auto) Result = Ur::View::MaybeVal(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<TOptional<const FString>>>);
    }
    {
        TOptional<FString&> Str;
        decltype(auto) Result = Ur::View::MaybeVal(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<TOptional<FString&>>>);
    }
    {
        TOptional<const FString&> Str;
        decltype(auto) Result = Ur::View::MaybeVal(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<TOptional<const FString&>>>);
    }
    // References
    {
        FString Str;
        decltype(auto) Result = Ur::View::MaybeRef(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<FString&>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FString&>);
    }
    {
        const FString Str;
        decltype(auto) Result = Ur::View::MaybeRef(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<const FString&>>);
    }
    {
        TOptional<FString> Str;
        decltype(auto) Result = Ur::View::MaybeRef(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<FString&>>);
    }
    {
        TOptional<const FString> Str;
        decltype(auto) Result = Ur::View::MaybeRef(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<const FString&>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), const FString&>);
    }
    {
        TOptional<FString&> Str;
        decltype(auto) Result = Ur::View::MaybeRef(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<FString&>>);
    }
    {
        TOptional<const FString&> Str;
        decltype(auto) Result = Ur::View::MaybeRef(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<const FString&>>);
    }
    // Optional
    {
        TOptional<FString> Str;
        decltype(auto) Result = Ur::View::Maybe(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<FString>>);
    }
    {
        TOptional<const FString> Str;
        decltype(auto) Result = Ur::View::Maybe(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<const FString>>);
    }
    {
        TOptional<FString&> Str;
        decltype(auto) Result = Ur::View::Maybe(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<FString&>>);
    }
    {
        TOptional<const FString&> Str;
        decltype(auto) Result = Ur::View::Maybe(Str);
        static_assert(std::is_same_v<decltype(Result), TMaybeView<const FString&>>);
    }
    {
        decltype(auto) Result = Ur::View::Maybe(TOptional<FString>{});
        static_assert(std::is_same_v<decltype(Result), TMaybeView<FString>>);
    }
    {
        decltype(auto) Result = Ur::View::Maybe(TOptional<const FString>{});
        static_assert(std::is_same_v<decltype(Result), TMaybeView<const FString>>);
    }
    {
        decltype(auto) Result = Ur::View::Maybe(TOptional<FString&>{});
        static_assert(std::is_same_v<decltype(Result), TMaybeView<FString&>>);
    }
    {
        decltype(auto) Result = Ur::View::Maybe(TOptional<const FString&>{});
        static_assert(std::is_same_v<decltype(Result), TMaybeView<const FString&>>);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaybeIterateValue, "UnrealRanges.Maybe.IterateValue", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaybeIterateValue::RunTest(const FString& Parameters)
{
    bool bResult = true;
    {
        decltype(auto) Result = Ur::View::MaybeVal(FString(TEXT("abba")));
        bResult &= Result.begin() != Result.end() && *Result.begin() == FString(TEXT("abba"));
    }
    {
        decltype(auto) Result = Ur::View::MaybeVal(FString(TEXT("abba")), false);
        bResult &= Result.begin() == Result.end();
    }
    {
        FString Str = FString(TEXT("abba"));
        decltype(auto) Result = Ur::View::MaybeVal(Str);
        bResult &= Result.begin() != Result.end() && *Result.begin() == FString(TEXT("abba"));
    }
    {
        FString Str = FString(TEXT("abba"));
        decltype(auto) Result = Ur::View::MaybeVal(Str, false);
        bResult &= Result.begin() == Result.end();
    }
    return bResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaybeIterateRef, "UnrealRanges.Maybe.IterateRef", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaybeIterateRef::RunTest(const FString& Parameters)
{
    bool bResult = true;
    {
        FString Str = FString(TEXT("abba"));
        decltype(auto) Result = Ur::View::MaybeRef(Str);
        bResult &= Result.begin() != Result.end() && *Result.begin() == FString(TEXT("abba"));
    }
    {
        FString Str = FString(TEXT("abba"));
        decltype(auto) Result = Ur::View::MaybeRef(Str, false);
        bResult &= Result.begin() == Result.end();
    }
    return bResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaybeIterateOptional, "UnrealRanges.Maybe.IterateOptional", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaybeIterateOptional::RunTest(const FString& Parameters)
{
    bool bResult = true;
    {
        TOptional<FString> Str = FString(TEXT("abba"));
        decltype(auto) Result = Ur::View::Maybe(Str);
        bResult &= Result.begin() != Result.end() && *Result.begin() == FString(TEXT("abba"));
    }
    {
        decltype(auto) Result = Ur::View::Maybe(TOptional<FString>(FString(TEXT("abba"))));
        bResult &= Result.begin() != Result.end() && *Result.begin() == FString(TEXT("abba"));
    }
    {
        TOptional<FString> Str;
        decltype(auto) Result = Ur::View::Maybe(Str);
        bResult &= Result.begin() == Result.end();
    }
    {
        decltype(auto) Result = Ur::View::Maybe(TOptional<FString>());
        bResult &= Result.begin() == Result.end();
    }
    {
        TOptional<FString> Str = FString(TEXT("abba"));
        decltype(auto) Result = Ur::View::Maybe(Str, false);
        bResult &= Result.begin() == Result.end();
    }
    {
        decltype(auto) Result = Ur::View::Maybe(TOptional<FString>(FString(TEXT("abba"))), false);
        bResult &= Result.begin() == Result.end();
    }
    return bResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaybeConstness, "UnrealRanges.Maybe.Constness", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaybeConstness::RunTest(const FString& Parameters)
{
    TMaybeView<FString> View = Ur::View::MaybeVal(FString());
    TMaybeView<FString>& ViewRef = View;
    const TMaybeView<FString>& ConstViewRef = View;

    static_assert(std::is_same_v<decltype(*View.begin()), FString&>);
    static_assert(std::is_same_v<decltype(*ViewRef.begin()), FString&>);
    static_assert(std::is_same_v<decltype(*ConstViewRef.begin()), const FString&>);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaybeInternalIteration, "UnrealRanges.Maybe.InternalIteration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaybeInternalIteration::RunTest(const FString& Parameters)
{
    auto View = Ur::View::MaybeVal(FString(TEXT("1"))).To<TArray>();

    return View.Num() == 1 && View[0] == FString(TEXT("1"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaybeInternalIterationEmpty, "UnrealRanges.Maybe.InternalIterationEmpty", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaybeInternalIterationEmpty::RunTest(const FString& Parameters)
{
    auto View = Ur::View::Maybe(TOptional<FString>()).To<TArray>();

    return View.Num() == 0;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaybeInternalIterationNullPtr, "UnrealRanges.Maybe.InternalIterationNullPtr", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaybeInternalIterationNullPtr::RunTest(const FString& Parameters)
{
    FString* Str = {};
    auto View = Ur::View::Maybe(Str).To<TArray>();

    return View.Num() == 0;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MaybeMixWithOtherMixins, "UnrealRanges.Maybe.MixWithOtherMixins", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MaybeMixWithOtherMixins::RunTest(const FString& Parameters)
{
    TOptional<FString> Str(FString(TEXT("abba")));
    auto View = Ur::View::Maybe(Str)
        .Transform([](const FString& Str) { return Str.Len(); })
        .To<TArray>();

    return View[0] == 4;
}

#endif