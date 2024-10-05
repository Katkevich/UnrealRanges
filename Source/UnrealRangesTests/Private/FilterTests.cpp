// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Filter, TestFilterEmptyRange)
{
    TArray<int32> From;

    auto Fn = [](int32 i) { return i > 0; };
    auto Result = Ref(From).Filter(Fn);

    static_assert(std::is_same_v<TFilterView<TRefView<TArray<int32>>, decltype(Fn)>, decltype(Result)>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Filter, TestFilterEven)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From).Filter([](int32 i) { return i % 2 == 0; });

    return EqualTo(Result, { 2,4 });
}

UR_TEST(UnrealRanges, Filter, TestFilterOutAll)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From).Filter([](int32 i) { return i > 5; });

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Filter, TestFilterInternalIteration)
{
    TArray<int32> From = { 1,2,3,4,5, 6 };

    auto Result = Ref(From).Filter([](int32 i) { return i % 2 == 0; }).To<TArray>();

    return EqualTo(Result, { 2, 4, 6 });
}


#endif