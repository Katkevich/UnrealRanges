// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;


UR_TEST(UnrealRanges, Count, TestRefView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Count() == 3;
}

UR_TEST(UnrealRanges, Count, TestEnumerateView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Enumerate().Count() == 3;
}

UR_TEST(UnrealRanges, Count, TestTransformVIew)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Transform([](int32 Num) { return Num; }).Count() == 3;
}

UR_TEST(UnrealRanges, Count, TestReverseView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Reverse().Count() == 3;
}

UR_TEST(UnrealRanges, Count, TestTakeViewGreaterThanArray)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(10).Count() == 3;
}

UR_TEST(UnrealRanges, Count, TestTakeViewLessThanArray)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(2).Count() == 2;
}

UR_TEST(UnrealRanges, Count, TestFilterView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Filter([](int32 Num) { return Num % 2 == 1; }).Count() == 2;
}

UR_TEST(UnrealRanges, Count, TestTakeWhileView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).TakeWhile([](int32 Num) { return Num <= 2; }).Count() == 2;
}

UR_TEST(UnrealRanges, Count, TestTakeWhilePlusEnumerateView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).TakeWhile([](int32 Num) { return Num <= 2; }).Enumerate().Count() == 2;
}

UR_TEST(UnrealRanges, CountIf, TestCountIf)
{
    TArray<int32> Array = { 1,2,3,4,5,6,7 };

    return Ref(Array).CountIf([](int32 Num) { return Num % 2 == 0; }) == 3;
}
#endif