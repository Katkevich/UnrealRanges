// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"
#include <forward_list>

using namespace Ur::View;

UR_TEST(UnrealRanges, StartsWith, TestNonEmptyViewStartsWithEmptyNonSizedRange)
{
    TArray<int32> Thiz = { 1,2,3 };
    std::forward_list<int32> That;

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}

UR_TEST(UnrealRanges, StartsWith, TestNonEmptyViewStartsWithEmptySizedRange)
{
    TArray<int32> Thiz = { 1,2,3 };
    TArray<int32> That;

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}

UR_TEST(UnrealRanges, StartsWith, TestEmptyViewStartsWithEmptyNonSizedRange)
{
    TArray<int32> Thiz;
    std::forward_list<int32> That;

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}

UR_TEST(UnrealRanges, StartsWith, TestEmptyViewStartsWithEmptySizedRange)
{
    TArray<int32> Thiz;
    TArray<int32> That;

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}


UR_TEST(UnrealRanges, StartsWith, TestNonEmptyViewStartsWithNonEmptyNonSizedRangeWithEqSize)
{
    TArray<int32> Thiz = { 1,2,3 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}

UR_TEST(UnrealRanges, StartsWith, TestNonEmptyViewStartsWithNonEmptySizedRangeWithEqSize)
{
    TArray<int32> Thiz = { 1,2,3 };
    TArray<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return bStartsWith;
}

UR_TEST(UnrealRanges, StartsWith, TestEmptyViewStartsWithNonEmptyNonSizedRange)
{
    TArray<int32> Thiz;
    std::forward_list<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

UR_TEST(UnrealRanges, StartsWith, TestEmptyViewIsntStartWithNonEmptySizedRange)
{
    TArray<int32> Thiz;
    TArray<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

UR_TEST(UnrealRanges, StartsWith, TestNonEmptyViewIsntStartWithNonEmptyNonSizedRange)
{
    TArray<int32> Thiz = { 1,2 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

UR_TEST(UnrealRanges, StartsWith, TestNonEmptyViewIsntStartWithNonEmptySizedRange)
{
    TArray<int32> Thiz = { 1,2 };
    TArray<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

UR_TEST(UnrealRanges, StartsWith, TestNonEmptyViewIsntStartWithNonEmptyNonSizedRangeSameSizeDiffValues)
{
    TArray<int32> Thiz = { 1,3,4 };
    std::forward_list<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

UR_TEST(UnrealRanges, StartsWith, TestNonEmptyViewIsntStartWithNonEmptySizedRangeSameSizeDiffValues)
{
    TArray<int32> Thiz = { 1,3,4 };
    TArray<int32> That = { 1,2,3 };

    bool bStartsWith = Ref(Thiz).StartsWith(That);

    return !bStartsWith;
}

#endif