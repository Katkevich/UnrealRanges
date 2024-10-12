// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/StringLiteral.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Zip, TestZipSingleEmptyRange)
{
    TArray<int32> From1 = { };

    auto Result = Zip(From1);

    static_assert(std::is_same_v<TTuple<int32&>, decltype(*Result.begin())>);

    return EqualTo(Result, std::initializer_list<TTuple<int32&>>{ });
}

UR_TEST(UnrealRanges, Zip, TestZipSingleNonEmptyRange)
{
    TArray<int32> From1 = { 1,2,3 };

    auto Result = Zip(From1);

    static_assert(std::is_same_v<TTuple<int32&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0]), ForwardAsTuple(From1[1]), ForwardAsTuple(From1[2]) });
}

UR_TEST(UnrealRanges, Zip, TestZipTwoRangesSecondIsShorter)
{
    TArray<int32> From1 = { 1,2 };
    TArray<FString> From2 = { TEXT("1"), TEXT("2"), TEXT("3")};

    auto Result = Zip(From1, From2);

    static_assert(std::is_same_v<TTuple<int32&, FString&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0]), ForwardAsTuple(From1[1], From2[1]) });
}

UR_TEST(UnrealRanges, Zip, TestZipTwoRangesFirstIsShorter)
{
    TArray<int32> From1 = { 1,2,3 };
    TArray<FString> From2 = { TEXT("1"), TEXT("2") };

    auto Result = Zip(From1, From2);

    static_assert(std::is_same_v<TTuple<int32&, FString&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0]), ForwardAsTuple(From1[1], From2[1]) });
}

UR_TEST(UnrealRanges, Zip, TestZipTwoRangesSameLength)
{
    TArray<int32> From1 = { 1,2 };
    TArray<FString> From2 = { TEXT("1"), TEXT("2") };

    auto Result = Zip(From1, From2);

    static_assert(std::is_same_v<TTuple<int32&, FString&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0]), ForwardAsTuple(From1[1], From2[1]) });
}

UR_TEST(UnrealRanges, Zip, TestZipWithTwoRangesSelfIsShorter)
{
    TArray<double> From1 = { 10.0, 20.0 };
    TArray<int32> From2 = { 1,2,3 };
    TArray<FString> From3 = { TEXT("1"), TEXT("2"), TEXT("3")};

    auto Result = Ref(From1)
        .ZipWith(From2, From3);

    static_assert(std::is_same_v<TTuple<double&, int32&, FString&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0], From3[0]), ForwardAsTuple(From1[1], From2[1], From3[1]) });
}

UR_TEST(UnrealRanges, Zip, TestZipWithTwoRangesSelfIsLonger)
{
    TArray<double> From1 = { 10.0, 20.0, 30.0 };
    TArray<int32> From2 = { 1,2 };
    TArray<FString> From3 = { TEXT("1"), TEXT("2") };

    auto Result = Ref(From1)
        .ZipWith(From2, From3);

    static_assert(std::is_same_v<TTuple<double&, int32&, FString&>, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0], From3[0]), ForwardAsTuple(From1[1], From2[1], From3[1]) });
}

UR_TEST(UnrealRanges, Zip, TestZipInternalIterationSelfIsLonger)
{
    TArray<double> From1 = { 10.0, 20.0, 30.0 };
    TArray<int32> From2 = { 1,2 };
    TArray<FString> From3 = { TEXT("1"), TEXT("2") };

    auto Result = Ref(From1)
        .ZipWith(From2, From3)
        .To<TArray>();

    static_assert(std::is_same_v<TTuple<double&, int32&, FString&>&, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0], From3[0]), ForwardAsTuple(From1[1], From2[1], From3[1]) });
}

UR_TEST(UnrealRanges, Zip, TestZipInternalIterationSelfIsShorter)
{
    TArray<double> From1 = { 10.0, 20.0 };
    TArray<int32> From2 = { 1,2,3 };
    TArray<FString> From3 = { TEXT("1"), TEXT("2") };

    auto Result = Ref(From1)
        .ZipWith(From2, From3)
        .To<TArray>();

    static_assert(std::is_same_v<TTuple<double&, int32&, FString&>&, decltype(*Result.begin())>);

    return EqualTo(Result, { ForwardAsTuple(From1[0], From2[0], From3[0]), ForwardAsTuple(From1[1], From2[1], From3[1]) });
}

UR_TEST(UnrealRanges, Zip, TestZipSized)
{
    TArray<double> From1 = { 10.0, 20.0 };
    TArray<int32> From2 = { 1,2,3 };
    TArray<FString> From3 = { TEXT("1"), TEXT("2") };

    auto Result = Ref(From1)
        .ZipWith(From2, From3);

    static_assert(std::is_same_v<TTuple<double&, int32&, FString&>, decltype(*Result.begin())>);

    return Result.Num() == 2;
}

UR_TEST(UnrealRanges, Zip, TestConcatZipAndSwizzle)
{
    auto Strs = TArray<FString>{ TEXT("0"),TEXT("1") };

    TMap<int32, FString> Map = Ur::View::Ref(Strs)      // "0","1"
        .ConcatWith(Ur::View::Single(TEXT("2")))        // "0","1","2"
        .ZipWith(Ur::View::Ints(0))                     // TTuple("0", 0), TTuple("1", 1), TTuple("2", 2)
        .Swizzle<1, 0>()                                // TTuple(0, "0"), TTuple(1, "1"), TTuple(2, "2")
        .To<TMap>();                                    // TMap{ TPair(0, "0"), TPair(1, "1"), TPair(2, "2") }

    return Ref(Map).EqualTo(std::array{ MakeTuple(0, TEXT("0"_s)), MakeTuple(1, TEXT("1"_s)), MakeTuple(2, TEXT("2"_s)) });
}

#endif