// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Fn/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;


UR_TEST(UnrealRanges, Sum, TestSumEmptyIntRange)
{
    TArray<int32> Array;

    return Ref(Array).Sum() == 0;
}

UR_TEST(UnrealRanges, Sum, TestSumEmptyStringRange)
{
    TArray<FString> Array;

    return Ref(Array).Sum() == FString();
}

UR_TEST(UnrealRanges, Sum, TestSumEmptyStringRangeUsingProjection)
{
    TArray<FString> Array;

    return Ref(Array).Sum(&FString::Len) == 0;
}

struct FItem
{
    int32 Value{};

private:
    friend int32 operator+(const FItem& Lhs, const FItem& Rhs)
    {
        return Lhs.Value + Rhs.Value;
    }

    friend int32 operator+(int32 Lhs, const FItem& Rhs)
    {
        return Lhs + Rhs.Value;
    }
};

UR_TEST(UnrealRanges, Sum, TestSumRangeWithCustomItem)
{
    TArray<FItem> Array = { {1}, {2}, {3} };

    return Ref(Array).Sum() == 6;
}

UR_TEST(UnrealRanges, Sum, TestSumOverRValues)
{
    const TCHAR* Chars = TEXT("abcdef");
    TArray<int32> Array = { 1,2,3 };

    auto Result = Ref(Array)
        .Transform([&](int32 Num) { return FString(Num, Chars); })
        .Sum();

    return Result == FString(TEXT("aababc"));
}

#endif