// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Traits.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;


UR_TEST(UnrealRanges, Maybe, FactoryCallResultType)
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

UR_TEST(UnrealRanges, Maybe, IterateValue)
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

UR_TEST(UnrealRanges, Maybe, IterateRef)
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

UR_TEST(UnrealRanges, Maybe, IterateOptional)
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

UR_TEST(UnrealRanges, Maybe, Constness)
{
    TMaybeView<FString> View = Ur::View::MaybeVal(FString());
    TMaybeView<FString>& ViewRef = View;
    const TMaybeView<FString>& ConstViewRef = View;

    static_assert(std::is_same_v<decltype(*View.begin()), FString&>);
    static_assert(std::is_same_v<decltype(*ViewRef.begin()), FString&>);
    static_assert(std::is_same_v<decltype(*ConstViewRef.begin()), const FString&>);

    return true;
}

UR_TEST(UnrealRanges, Maybe, InternalIterate)
{
    auto View = Ur::View::MaybeVal(FString(TEXT("1"))).To<TArray>();

    return View.Num() == 1 && View[0] == FString(TEXT("1"));
}

UR_TEST(UnrealRanges, Maybe, InternalIterationEmpty)
{
    auto View = Ur::View::Maybe(TOptional<FString>()).To<TArray>();

    return View.Num() == 0;
}

UR_TEST(UnrealRanges, Maybe, InternalIterationNullPtr)
{
    FString* Str = {};
    auto View = Ur::View::Maybe(Str).To<TArray>();

    return View.Num() == 0;
}

UR_TEST(UnrealRanges, Maybe, MixWithOtherMixinsInternalIteration)
{
    TOptional<FString> Str(FString(TEXT("abba")));
    auto View = Ur::View::Maybe(Str)
        .Transform([](const FString& Str) { return Str.Len(); })
        .To<TArray>();

    return View[0] == 4;
}

UR_TEST(UnrealRanges, Maybe, MixWithConcat)
{
    TArray<FString> Array = { TEXT("1"), TEXT("abba1") , TEXT("abba2"), TEXT("2") };

    auto Result = Ref(Array)
        .Filter([](const FString& Str) { return Str.Len() > 1; })
        .ConcatWith(MaybeVal(FString(TEXT("single1"))), Maybe(TOptional<FString&>()), MaybeVal(FString(TEXT("single2"))))
        .Transform([](FString& Str) { return Str.RightChop(1); });

    return EqualTo(Result, { TEXT("bba1") , TEXT("bba2"), TEXT("ingle1"), TEXT("ingle2") });
}

#endif