// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, DropWhile, TestDropWhileEmptyRange)
{
    TArray<int32> From;

    auto Result = Ref(From).DropWhile([](int32 Num) { return Num % 2 == 0; });

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, DropWhile, TestDropAll)
{
    TArray<int32> From = { 2,2,2 };

    const auto Result = Ref(From).DropWhile([](int32 Num) { return Num % 2 == 0; });

    return EqualTo(Result, std::initializer_list<int32>{});
}

UR_TEST(UnrealRanges, DropWhile, TestDropNothing)
{
    TArray<int32> From = { 2,2,2 };

    const auto Result = Ref(From).DropWhile([](int32 Num) { return Num % 2 == 1; });

    return EqualTo(Result, { 2,2,2 });
}

UR_TEST(UnrealRanges, DropWhile, TestDropSome)
{
    TArray<int32> From = { 2,2,3 };

    const auto Result = Ref(From).DropWhile([](int32 Num) { return Num % 2 == 0; });

    return EqualTo(Result, { 3 });
}

UR_TEST(UnrealRanges, DropWhile, TestDropRValue)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    const auto Result = Ref(From)
        .Transform([](const FString& Str) { return Str + Str; })
        .DropWhile([](const FString& Str) { return Str.Len() == 2; });

    static_assert(std::is_same_v<decltype(*Result.begin()), FString>);

    return EqualTo(Result, { FString(TEXT("3333")) });
}

UR_TEST(UnrealRanges, DropWhile, TestDropLValue)
{
    TArray<FString> From = { TEXT("1"), TEXT("2"), TEXT("3"), TEXT("33") };

    const auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .DropWhile([](const FString& Str) { return Str == TEXT("1"); });

    static_assert(std::is_same_v<decltype(*Result.begin()), const FString&>);

    return EqualTo(Result, { TEXT("2"), TEXT("3") });
}

UR_TEST(UnrealRanges, DropWhile, TestDropLValueAfterReverse)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")), FString(TEXT("33")) };

    const auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .Reverse()
        .DropWhile([](const FString& Str) { return Str == TEXT("3"); });

    static_assert(std::is_same_v<decltype(*Result.begin()), const FString&>);

    return EqualTo(Result, { TEXT("2"), TEXT("1") });
}

UR_TEST(UnrealRanges, DropWhile, TestDropWhileInternalIteration)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .DropWhile([](const FString& Str) { return Str == TEXT("1"); })
        .To<TArray>();

    return EqualTo(Result, { TEXT("2") });
}

UR_TEST(UnrealRanges, DropWhile, TestDropWhileNothingToDrop)
{
    TArray<FString> From = { TEXT("1"), TEXT("2"), TEXT("33") };

    auto Result = Ref(From)
        .DropWhile([](const FString& Str) { return Str.Len() == 3; })
        .To<TArray>();

    return EqualTo(Result, { TEXT("1"), TEXT("2"), TEXT("33") });
}

#endif