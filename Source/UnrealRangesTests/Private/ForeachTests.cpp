// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Foreach, TestForeachWithIteratorUnderTheHood)
{
    TArray<int32> From1 = { 1,2,3,4,5,6,7,8 };
    TArray<int32> Result;

    Ref(From1)
        .Filter(Ur::Fn::Even)
        .Take(3)
        .Foreach([&](int32& Num)
            {
                Result.Add(Num);
            });

    return EqualTo(Result, { 2,4,6 });
}


UR_TEST(UnrealRanges, Foreach, TestForeachWithCursorUnderTheHood)
{
    TArray<int32> From1 = { 1,2,3,4,5,6,7,8 };
    TArray<int32> From2 = { 1,2,3,4,5,6,7 };
    TArray<TTuple<int32, int32>> Result;

    Ref(From1)
        .Filter(Ur::Fn::Even)
        .ZipWith(From2)
        .Foreach([&](TTuple<int32&, int32&> Pair)
            {
                Result.Add(MakeTuple(Pair.Key, Pair.Value));
            });

    return EqualTo(Result, { MakeTuple(2,1), MakeTuple(4,2), MakeTuple(6,3), MakeTuple(8,4) });
}

#endif