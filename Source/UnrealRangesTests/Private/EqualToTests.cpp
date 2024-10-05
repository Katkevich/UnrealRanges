// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"
#include <forward_list>

using namespace Ur::View;

UR_TEST(UnrealRanges, EqualTo, TestNonEmptyViewNotEqualToEmptyNonSizedRange)
{
    TArray<int32> Thiz = { 1,2,3 };
    std::forward_list<int32> That;

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestNonEmptyViewNotEqualToEmptySizedRange)
{
    TArray<int32> Thiz = { 1,2,3 };
    TArray<int32> That;

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestEmptyViewEqualToEmptyNonSizedRange)
{
    TArray<int32> Thiz;
    std::forward_list<int32> That;

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestEmptyViewEqualToEmptySizedRange)
{
    TArray<int32> Thiz;
    TArray<int32> That;

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return bEqualTo;
}


UR_TEST(UnrealRanges, EqualTo, TestNonEmptyViewEqualToNonEmptyNonSizedRangeWithEqSize)
{
    TArray<int32> Thiz = { 1,2,3 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestNonEmptyViewEqualToNonEmptySizedRangeWithEqSize)
{
    TArray<int32> Thiz = { 1,2,3 };
    TArray<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestNonEmptyViewEqualToNonEmptySizedRangeWithEqSizeWithDiffItemType)
{
    TArray<int32> Thiz = { 1,2,3 };
    TArray<float> That = { 1.f,2.f,3.f };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestEmptyViewNotEqualToNonEmptyNonSizedRange)
{
    TArray<int32> Thiz;
    std::forward_list<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestEmptyViewNotEqualToNonEmptySizedRange)
{
    TArray<int32> Thiz;
    TArray<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestNonEmptyViewNotEqualToNonEmptyNonSizedRange)
{
    TArray<int32> Thiz = { 1,2 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestNonEmptyViewNotEqualToNonEmptySizedRange)
{
    TArray<int32> Thiz = { 1,2 };
    TArray<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestNonEmptyViewNotEqualToNonEmptyNonSizedRangeSameSizeDiffValues)
{
    TArray<int32> Thiz = { 1,3,4 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

UR_TEST(UnrealRanges, EqualTo, TestNonEmptyViewNotEqualToNonEmptySizedRangeSameSizeDiffValues)
{
    TArray<int32> Thiz = { 1,3,4 };
    TArray<int32> That = { 1,2,3 };

    bool bEqualTo = Ref(Thiz).EqualTo(That);

    return !bEqualTo;
}

#endif