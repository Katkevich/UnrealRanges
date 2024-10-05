// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Find, TestFindEmptyRange)
{
    TArray<int32> From;

    auto Result = Ref(From).Find(1);

    static_assert(std::is_same_v<TOptional<int32&>, decltype(Result)>);

    return !Result.IsSet();
}

UR_TEST(UnrealRanges, Find, TestFindWithImplicitConvertsion)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("aa")) , FString(TEXT("aaa")), FString(TEXT("aaa")) };

    auto Result = Ref(From).Find(FStringView(TEXT("aa")));

    static_assert(std::is_same_v<TOptional<FString&>, decltype(Result)>);

    return Result.IsSet() && *Result == FString(TEXT("aa"));
}

UR_TEST(UnrealRanges, Find, TestFindReturnsVeryFirstMatch)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("aa")) , FString(TEXT("aa")), FString(TEXT("aa")) };

    TOptional<FString&> Result = Ref(From).Find(FStringView(TEXT("aa")));

    return Result.GetPtrOrNull() == std::addressof(From[1]);
}

UR_TEST(UnrealRanges, Find, TestFindWithProjectionWhichReturnsRValue)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("a")), FString(TEXT("b")) };

    TOptional<FString&> Result = Ref(From).Find(TEXT("bb"), [](const FString& Str) { return Str + Str; });

    return Result.GetPtrOrNull() == std::addressof(From[2]);
}

UR_TEST(UnrealRanges, Find, TestFindWithProjectionBeingMemberPtr)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("aa")), FString(TEXT("b")), FString(TEXT("b")) };

    TOptional<FString&> Result = Ref(From).Find(2, &FString::Len);

    return Result.GetPtrOrNull() == std::addressof(From[1]);
}

UR_TEST(UnrealRanges, Find, TestFindByPredicate)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("23")), FString(TEXT("233")), FString(TEXT("2333")) };

    TOptional<FString&> Result = Ref(From).FindBy([](FString& Str) { return Str == TEXT("23"); });

    return Result.GetPtrOrNull() == std::addressof(From[1]);
}

UR_TEST(UnrealRanges, Find, TestFindCalledOnConstRangeReturnsConstResult)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("23")), FString(TEXT("233")), FString(TEXT("2335")), FString(TEXT("2334")) };
    const TRefView<TArray<FString>> RefView = Ref(From);

    TOptional<const FString&> Result0 = RefView.Find(TEXT("233"));
    TOptional<const FString&> Result1 = RefView.Find(3, &FString::Len);
    TOptional<const FString&> Result2 = RefView.FindBy([](const FString& Str) { return Str == TEXT("23"); });

    return Result0.GetPtrOrNull() == std::addressof(From[2]) && Result1.GetPtrOrNull() == std::addressof(From[2]) && Result2.GetPtrOrNull() == std::addressof(From[1]);
}

UR_TEST(UnrealRanges, Find, TestContainsEmptyRange)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("23")), FString(TEXT("233")), FString(TEXT("2335")), FString(TEXT("2334")) };
    const TRefView<TArray<FString>> RefView = Ref(From);
    TRefView<TArray<FString>> ConstRefView = Ref(From);

    bool Result0 = RefView.Contains(TEXT("233"));
    bool Result1 = ConstRefView.Contains(TEXT("233"));
    bool Result2 = RefView.Contains(3, &FString::Len);
    bool Result3 = ConstRefView.ContainsBy([](const FString& Str) { return Str == TEXT("23"); });

    bool Result4 = RefView.Contains(TEXT("_233"));
    bool Result5 = ConstRefView.Contains(TEXT("_233"));
    bool Result6 = RefView.Contains(10, &FString::Len);
    bool Result7 = ConstRefView.ContainsBy([](const FString& Str) { return Str == TEXT("_23"); });


    return Result0 && Result1 && Result2 && Result3 && !Result4 && !Result5 && !Result6 && !Result7;
}

#endif