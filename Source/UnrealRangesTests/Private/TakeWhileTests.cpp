// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, TakeWhile, TestTakeWhileEmptyRange)
{
    TArray<int32> From;

    auto Result = Ref(From).TakeWhile([](int32 Num) { return Num % 2 == 0; });

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, TakeWhile, TestTakeAll)
{
    TArray<int32> From = { 2,2,2 };

    const auto Result = Ref(From).TakeWhile([](int32 Num) { return Num % 2 == 0; });

    return EqualTo(Result, { 2,2,2 });
}

UR_TEST(UnrealRanges, TakeWhile, TestTakeNothing)
{
    TArray<int32> From = { 2,2,2 };

    const auto Result = Ref(From).TakeWhile([](int32 Num) { return Num % 2 == 1; });

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, TakeWhile, TestTakeSome)
{
    TArray<int32> From = { 2,2,3 };

    const auto Result = Ref(From).TakeWhile([](int32 Num) { return Num % 2 == 0; });

    return EqualTo(Result, { 2,2 });
}

UR_TEST(UnrealRanges, TakeWhile, TestTakeRValue)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    const auto Result = Ref(From)
        .Transform([](const FString& Str) { return Str + Str; })
        .TakeWhile([](const FString& Str) { return Str.Len() == 2; });

    static_assert(std::is_same_v<decltype(*Result.begin()), FString>);

    return EqualTo(Result, { FString(TEXT("11")), FString(TEXT("22")) });
}

UR_TEST(UnrealRanges, TakeWhile, TestTakeLValue)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    const auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .TakeWhile([](const FString& Str) { return Str.Len() == 2; });

    static_assert(std::is_same_v<decltype(*Result.begin()), const FString&>);

    return EqualTo(Result, std::initializer_list<FString>{ });
}

UR_TEST(UnrealRanges, TakeWhile, TestTakeLValueAfterReverse)
{
    TArray<FString> From = { TEXT("1"), TEXT("2"), TEXT("33") };

    const auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .Reverse()
        .TakeWhile([](const FString& Str) { return Str.Len() == 1; });

    static_assert(std::is_same_v<decltype(*Result.begin()), const FString&>);

    return EqualTo(Result, { TEXT("2"), TEXT("1")});
}

UR_TEST(UnrealRanges, TakeWhile, TestTakeInternalIteration)
{
    TArray<FString> From = { FString(TEXT("11")), FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str.Len() == 1; })
        .TakeWhile([](const FString& Str) { return Str == TEXT("1"); })
        .To<TArray>();

    return EqualTo(Result, { TEXT("1") });
}

UR_TEST(UnrealRanges, TakeWhile, TestTakeInternalIterationEmptyResult)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("2")) , FString(TEXT("33")) };

    auto Result = Ref(From)
        .TakeWhile([](const FString& Str) { return Str.Len() == 3; })
        .To<TArray>();

    return Result.Num() == 0;
}

UR_TEST(UnrealRanges, TakeWhile, TestTakeInternalIterationEmptySource)
{
    TArray<FString> From;

    auto Result = Ref(From)
        .TakeWhile([](const FString& Str) { return Str.Len() == 3; })
        .To<TArray>();

    return Result.Num() == 0;
}

UR_TEST(UnrealRanges, TakeWhile, TestTakeWhileOnDifferentViews)
{
    TArray<FString> From = { FString(TEXT("00")), FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("33")), FString(TEXT("4")) };

    bool bResult = true;
    {
        auto Result = Ref(From)
            .TakeWhile([](const FString& Str) { return Str.Len() == 2; });

        bResult &= Result.Count() == 1;
    }
    {
        auto Result = Ref(From)
            .Filter([](const FString& Str) { return Str.Len() == 1; })
            .TakeWhile([](const FString& Str) { return Str.Len() == 1; });

        bResult &= EqualTo(Result, { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("4")) });
    }
    {
        auto Result = Ref(From)
            .Transform([](const FString& Str) { return Str + Str; })
            .TakeWhile([](const FString& Str) { return Str.Len() == 4; });

        bResult &= EqualTo(Result, { FString(TEXT("0000")) });
    }
    {
        auto Result = Ref(From)
            .Reverse()
            .TakeWhile([](const FString& Str) { return Str.Len() == 1; });

        bResult &= EqualTo(Result, { FString(TEXT("4")) });
    }
    {
        auto Result = Ref(From)
            .Reverse()
            .Enumerate()
            .TakeWhile([](FIndexed<FString&> Str) { return Str.Value.Len() == 1; });

        bResult &= EqualTo(Result, { FIndexed<FString&>(From[4], 0) });
    }
    {
        auto Result = Ref(From)
            .Filter([](const FString& Str) { return Str.Len() == 1; })
            .Take(10)
            .TakeWhile([](const FString& Str) { return Str.Len() == 1; });

        bResult &= EqualTo(Result, { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("4")) });
    }
    {
        auto Result = Ref(From)
            .Filter([](const FString& Str) { return Str.Len() == 1; })
            .TakeWhile([](const FString& Str) { return Str.Len() == 1; })
            .TakeWhile([](const FString& Str) { return Str[0] == TEXT('1'); });

        bResult &= EqualTo(Result, { FString(TEXT("1")) });
    }

    return bResult;
}

#endif