// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "Containers/Array.h"

UR_TEST(UnrealRanges, MinMax, TestMinOnEmptyRangeReturnsEmptyOptional)
{
    TArray<int32> From;

    TOptional<int32&> Result = Ur::View::Ref(From).Min();

    return !Result.IsSet();
}

UR_TEST(UnrealRanges, MinMax, TestMaxOnEmptyRangeReturnsEmptyOptional)
{
    TArray<int32> From;

    TOptional<int32&> Result = Ur::View::Ref(From).Max();

    return !Result.IsSet();
}

UR_TEST(UnrealRanges, MinMax, TestMinMaxOnEmptyRangeReturnsEmptyOptional)
{
    TArray<int32> From;

    Ur::TMinMax<int32&> Result = Ur::View::Ref(From).MinMax();

    return !Result.Min.IsSet() && !Result.Max.IsSet();
}

UR_TEST(UnrealRanges, MinMax, TestMinReturnsReferenceFstMin)
{
    TArray<int32> From = { 1, 2, 3 };

    TOptional<int32&> Result = Ur::View::Ref(From).Min();

    return Result.IsSet() && Result.GetValue() == 1;
}

UR_TEST(UnrealRanges, MinMax, TestMinReturnsReferenceLstMin)
{
    TArray<int32> From = { 2, 3, 1 };

    TOptional<int32&> Result = Ur::View::Ref(From).Min();

    return Result.IsSet() && Result.GetValue() == 1;
}

UR_TEST(UnrealRanges, MinMax, TestMaxReturnsReferenceFstMax)
{
    TArray<int32> From = { 3, 2, 1 };

    TOptional<int32&> Result = Ur::View::Ref(From).Max();

    return Result.IsSet() && Result.GetValue() == 3;
}

UR_TEST(UnrealRanges, MinMax, TestMaxReturnsReferenceLstMax)
{
    TArray<int32> From = { 1, 2, 3 };

    TOptional<int32&> Result = Ur::View::Ref(From).Max();

    return Result.IsSet() && Result.GetValue() == 3;
}

UR_TEST(UnrealRanges, MinMax, TestMinMaxReturnsReferenceSingleElement)
{
    TArray<int32> From = { 1 };

    Ur::TMinMax<int32&> Result = Ur::View::Ref(From).MinMax();

    return Result.Min.IsSet() && Result.Max.IsSet() && Result.Min.GetValue() == 1 && Result.Max.GetValue() == 1;
}

UR_TEST(UnrealRanges, MinMax, TestMinMaxReturnsReferenceDiffElements)
{
    TArray<int32> From = { 1,2,3 };

    Ur::TMinMax<int32&> Result = Ur::View::Ref(From).MinMax();

    return Result.Min.IsSet() && Result.Max.IsSet() && Result.Min.GetValue() == 1 && Result.Max.GetValue() == 3;
}

UR_TEST(UnrealRanges, MinMax, TestMinMaxReturnsReferenceSeveralDuplicates)
{
    TArray<int32> From = { 1,1,1 };

    Ur::TMinMax<int32&> Result = Ur::View::Ref(From).MinMax();

    return Result.Min.IsSet() && Result.Max.IsSet() && Result.Min.GetValue() == 1 && Result.Max.GetValue() == 1 && Result.Min.GetPtrOrNull() == Result.Max.GetPtrOrNull();
}


UR_TEST(UnrealRanges, MinMax, TestMinBy)
{
    TArray<FString> From = { FString(TEXT("aa")), FString(TEXT("aaa")) , FString(TEXT("a")) };

    auto Result = Ur::View::Ref(From)
        .Min([](FString& Str) { return Str.Len(); });

    static_assert(std::is_same_v<TOptional<FString&>, decltype(Result)>, "err");

    return Result.GetPtrOrNull() == std::addressof(From[2]);
}

UR_TEST(UnrealRanges, MinMax, TestMaxBy)
{
    TArray<FString> From = { FString(TEXT("aa")), FString(TEXT("aaa")) , FString(TEXT("a")) };

    auto Result = Ur::View::Ref(From)
        .Max([](FString& Str) { return Str.Len(); });

    static_assert(std::is_same_v<TOptional<FString&>, decltype(Result)>, "err");

    return Result.GetPtrOrNull() == std::addressof(From[1]);
}

UR_TEST(UnrealRanges, MinMax, TestMaxByMemberPtrProjection)
{
    TArray<FString> From = { FString(TEXT("aa")), FString(TEXT("aaa")) , FString(TEXT("a")) };

    auto Result = Ur::View::Ref(From)
        .Max(&FString::Len);

    static_assert(std::is_same_v<TOptional<FString&>, decltype(Result)>, "err");

    return Result.GetPtrOrNull() == std::addressof(From[1]);
}

UR_TEST(UnrealRanges, MinMax, TestMinMaxBy)
{
    TArray<FString> From = { FString(TEXT("aa")), FString(TEXT("aaa")) , FString(TEXT("a")) };

    auto Result = Ur::View::Ref(From)
        .MinMax([](FString& Str) { return Str.Len(); });

    static_assert(std::is_same_v<Ur::TMinMax<FString&>, decltype(Result)>, "err");

    return Result.Min.GetPtrOrNull() == std::addressof(From[2]) && Result.Max.GetPtrOrNull() == std::addressof(From[1]);
}

UR_TEST(UnrealRanges, MinMax, TestMaxRValueResult)
{
    const TCHAR* Str = TEXT("12345");
    TArray<int32> Lens = { 1,3,2,1 };

    auto Result = Ur::View::Ref(Lens)
        .Transform([&](int32 Len) { return FString(Len, Str); })
        .Max([](const FString& Str) { return Str.Len(); });

    static_assert(std::is_same_v<TOptional<FString>, decltype(Result)>, "err");

    return Result.IsSet() && *Result == TEXT("123");
}

UR_TEST(UnrealRanges, MinMax, TestOperatorLessIsRequired)
{
    struct Foo
    {
        auto operator<(const Foo&) const = delete;
    };
    TArray<Foo> Foos;

    //Ur::View::Ref(Foos).Max(); // doesn't compile!

    return true;
}

#endif