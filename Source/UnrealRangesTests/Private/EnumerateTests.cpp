// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Filter, TestEnumerateEmptyRange)
{
    TArray<int32> From;

    auto Result = Ref(From).Enumerate();

    static_assert(std::is_same_v<TEnumerateView<TRefView<TArray<int32>>, int32>, decltype(Result)>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Filter, TestEnumerateLValues)
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

UR_TEST(UnrealRanges, Filter, TestEnumerateRValues)
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

UR_TEST(UnrealRanges, Filter, TestEnumerateRValuesWithDifferentDefaults)
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

UR_TEST(UnrealRanges, Filter, TestEnumeratedInternalIteration)
{
    TArray<int32> From = { 1,2,3 };

    auto Result = Enumerate(From, 2, 3)
        .Filter([&](FIndexed<int32&> Num) { return Num.Index % 2 == 0; })
        .To<TArray>();

    return EqualTo(Result, { FIndexed<int32&>(From[0], 2), FIndexed<int32&>(From[2], 8) });
}

template<typename T>
struct foo;

UR_TEST(UnrealRanges, Filter, TestEnumerateStructuredBinding)
{
    FString Str;

    FIndexed<FString> Indexed;
    const FIndexed<FString> ConstIndexed;

    FIndexed<FString&> IndexedRef(Str, 0);
    const FIndexed<FString&> ConstIndexedRef(Str, 0);

    FIndexed<const FString&> DeepConstIndexedRef(Str, 0);
    const FIndexed<const FString&> ConstDeepConstIndexedRef(Str, 0);

    static_assert(std::is_same_v<decltype(Indexed.get<0>()), FString&>);
    static_assert(std::is_same_v<decltype(ConstIndexed.get<0>()), const FString&>);

    static_assert(std::is_same_v<decltype(IndexedRef.get<0>()), FString&>);
    static_assert(std::is_same_v<decltype(ConstIndexedRef.get<0>()), FString&>); // no deep const

    static_assert(std::is_same_v<decltype(DeepConstIndexedRef.get<0>()), const FString&>);
    static_assert(std::is_same_v<decltype(ConstDeepConstIndexedRef.get<0>()), const FString&>);

    static_assert(std::is_same_v<decltype(FIndexed<FString>{}.get<0>()), FString&&>);
    static_assert(std::is_same_v<decltype(FIndexed<FString&>{Str, 0}.get<0>()), FString&>);

    static_assert(std::is_same_v<decltype(FIndexed<const FString>{}.get<0>()), const FString&&>);
    static_assert(std::is_same_v<decltype(FIndexed<const FString&>{Str, 0}.get<0>()), const FString&>);

    static_assert(std::is_same_v<decltype(TTuple<FString>{}.Get<0>()), FString&&>);
    static_assert(std::is_same_v<decltype(TTuple<FString&>{}.Get<0>()), FString&>);

    static_assert(std::is_same_v<decltype(TTuple<const FString>{}.Get<0>()), const FString&&>);
    static_assert(std::is_same_v<decltype(TTuple<const FString&>{}.Get<0>()), const FString&>);


    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("b")), FString(TEXT("c")) };

    for (const auto& [Value, Index] : Enumerate(From, 2, 3)) // *Iter -> FIndexed<FString&> + lifetime extension
    {
        static_assert(std::is_same_v<decltype(Value), FString&>); // no deep const
    }

    for (auto [Value, Index] : Transform(From, [](FString& Str) { return Str; }).Enumerate()) // *Iter -> FIndexed<FString>
    {
        static_assert(std::is_same_v<decltype(Value), FString>);
    }

    for (const auto& [Value, Index] : Transform(From, [](FString& Str) { return Str; }).Enumerate()) // *Iter -> FIndexed<FString> + lifetime extension
    {
        // not exactly sure why we have const FString (and no FString) here but TTuple and std::tuple has the same bahavior...
        // structured binding is a weird creature so just keep it like that
        static_assert(std::is_same_v<decltype(Value), const FString>);
    }

    for (const auto& [Value] : Transform(From, [](FString& Str) { return TTuple<FString>(Str); })) // *Iter -> FIndexed<FString> + lifetime extension
    {
        static_assert(std::is_same_v<decltype(Value), const FString>);
    }

    for (const auto& [Value] : Transform(From, [](FString& Str) { return std::tuple<FString>(Str); })) // *Iter -> FIndexed<FString> + lifetime extension
    {
        static_assert(std::is_same_v<decltype(Value), const FString>);
    }

    return true;
}

#endif