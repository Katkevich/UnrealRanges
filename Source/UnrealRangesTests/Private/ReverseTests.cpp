// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Reverse, TestReverseEmpty)
{
    TArray<int32> From;

    auto Result = Ref(From).Reverse();

    static_assert(std::is_same_v<TReverseView<TRefView<TArray<int32>>>, decltype(Result)>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Reverse, TestReverseNonEmpty)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From).Reverse();

    return EqualTo(Result, { 5,4,3,2,1 });
}

UR_TEST(UnrealRanges, Reverse, TestDoubleReverseChangeNothing)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From).Reverse().Reverse();

    return EqualTo(Result, { 1,2,3,4,5 });
}

UR_TEST(UnrealRanges, Reverse, TestDoubleReverseWithTransformInTheMiddleChangeNothing)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From)
        .Reverse()
        .Transform([](int32 Num) { return Num * Num; })
        .Reverse();

    return EqualTo(Result, { 1,4,9,16,25 });
}

UR_TEST(UnrealRanges, Reverse, TestReverseAfterFilter)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From)
        .Filter([](int32 Num) { return Num % 2 == 1; })
        .Reverse();

    return EqualTo(Result, { 5, 3, 1 });
}

#endif