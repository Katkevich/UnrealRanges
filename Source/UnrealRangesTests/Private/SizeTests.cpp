// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur;
using namespace Ur::View;

UR_TEST(UnrealRanges, Size, TestRefView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Num() == 3;
}

UR_TEST(UnrealRanges, Size, TestEnumerateView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Enumerate().Num() == 3;
}

UR_TEST(UnrealRanges, Size, TestTransformVIew)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Transform([](int32 Num) { return Num; }).Num() == 3;
}

UR_TEST(UnrealRanges, Size, TestReverseView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Reverse().Num() == 3;
}

UR_TEST(UnrealRanges, Size, TestTakeViewGreaterThanArray)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(10).Num() == 3;
}

UR_TEST(UnrealRanges, Size, TestTakeViewLessThanArray)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(2).Num() == 2;
}

UR_TEST(UnrealRanges, Size, TestSizedRangeConcept)
{
    TArray<int32> Array = { 1,2,3 };
    FString Str;

    static_assert(SizedRange<TArray<int32>>);
    static_assert(SizedRange<decltype(Ref(Array))>);
    static_assert(SizedRange<decltype(Ref(Array).Reverse())>);
    static_assert(SizedRange<decltype(Ref(Array).Enumerate())>);
    static_assert(SizedRange<decltype(Ref(Array).Transform([](int32 Num) { return Num; }))>);
    static_assert(SizedRange<decltype(Ref(Array).Take(5))>);
    static_assert(SizedRange<decltype(Ref(Str))>);
    static_assert(SizedRange<decltype(Ref(Str).Reverse())>);
    static_assert(SizedRange<decltype(Ref(Str).Enumerate())>);
    static_assert(SizedRange<decltype(Ref(Str).Take(5))>);

    static_assert(!SizedRange<decltype(Ref(Array).TakeWhile([](int32 Num) { return Num > 0; }))>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }))>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }).Enumerate())>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }).Reverse())>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }).Take(5))>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }).Transform([](int32 Num) { return Num; }))>);
    static_assert(!SizedRange<decltype(Ref(Array).Filter([](int32 Num) { return Num > 0; }).Reverse().Take(5))>);

    return true;
}

#endif