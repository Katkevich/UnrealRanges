// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Drop, TestDropEmptyRange)
{
    TArray<int32> From;

    auto Result = Ref(From).Drop(10);

    static_assert(std::is_same_v<TDropView<TRefView<TArray<int32>>, int32>, decltype(Result)>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Drop, TestDropSkipAllElements)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From).Drop(10);

    static_assert(std::is_same_v<TDropView<TRefView<TArray<int32>>, int32>, decltype(Result)>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Drop, TestDropIsSizedIfUnderlyingViewIsSized)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Num = Ref(From).Drop(3).Num();

    return Num == 2;
}

UR_TEST(UnrealRanges, Drop, TestDropIsNotSizedIfUnderlyingViewIsNotSized)
{
    TArray<int32> From = { 1,2,3,4,5,6 };

    auto Result = Ref(From).Filter(Ur::Fn::Even).Drop(1);

    static_assert(!decltype(Result)::IsSized);

    return Result.Count() == 2 && EqualTo(Result, { 4,6 });
}

UR_TEST(UnrealRanges, Drop, TestDropOofRValueItem)
{
    const TCHAR* Chars = TEXT("abcdef");
    TArray<int32> From = { 1,2,3,4,5,6 };

    auto Result = Ref(From)
        .Transform([&](int32 Num) { return FString(Num, Chars); })
        .Drop(4);

    return EqualTo(Result, { TEXT("abcde"), TEXT("abcdef") });
}

#endif