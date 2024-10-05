// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Take, TestTakeEmptyRange)
{
    TArray<int32> From;

    auto Result = Ref(From).Take(10);

    static_assert(std::is_same_v<TTakeView<TRefView<TArray<int32>>, int32>, decltype(Result)>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Take, TestTakeGettingMoreThanThereIs)
{
    TArray<int32> From = { 1,2,3 };

    const auto Result = Ref(From).Take(10);

    return EqualTo(Result, { 1, 2, 3});
}

UR_TEST(UnrealRanges, Take, TestTakeGettingLessThanThereIs)
{
    TArray<int32> From = { 1,2,3,4,5 };

    const auto Result = Ref(From).Take(3);

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, Take, TestTakeGettingZeroElements)
{
    TArray<int32> From = { 1,2,3,4,5 };

    const auto Result = Ref(From).Take(0);

    return Result.cbegin() == Result.cend();
}

UR_TEST(UnrealRanges, Take, TestTakeWithOtherViewsInternalIteration)
{
    TArray<int32> From = { 1,2,3,4,5,6,7,8,9 };

    const auto Result = Ref(From)
        .Filter([](int32 Num) { return Num % 2 == 0; })
        .Transform([](int32 Num) { return Num * Num; })
        .Reverse()
        .Take(3)
        .Enumerate(1)
        .To<TArray>();

    return EqualTo(Result, { FIndexed<int32>(64, 1), FIndexed<int32>(36, 2), FIndexed<int32>(16, 3) });
}

UR_TEST(UnrealRanges, Take, TestTakeWithOtherViews)
{
    TArray<int32> From = { 1,2,3,4,5,6,7,8,9 };

    const auto Result = Ref(From)
        .Filter([](int32 Num) { return Num % 2 == 0; })
        .Transform([](int32 Num) { return Num * Num; })
        .Reverse()
        .Take(3)
        .Enumerate(1);

    return EqualTo(Result, { FIndexed<int32>(64, 1), FIndexed<int32>(36, 2), FIndexed<int32>(16, 3) });
}

#endif