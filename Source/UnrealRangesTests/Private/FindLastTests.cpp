// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Find, TestFindLastEmptyRange)
{
    TArray<int32> From;

    auto Result = Ref(From).FindLast(1);

    static_assert(std::is_same_v<TOptional<int32&>, decltype(Result)>);

    return !Result.IsSet();
}
UR_TEST(UnrealRanges, Find, TestFindLastWithImplicitConvertsion)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("aa")) , FString(TEXT("aaa")), FString(TEXT("aaa")) };

    auto Result = Ref(From).FindLast(FStringView(TEXT("aa")));

    static_assert(std::is_same_v<TOptional<FString&>, decltype(Result)>);

    return Result.IsSet() && *Result == FString(TEXT("aa"));
}

UR_TEST(UnrealRanges, Find, TestFindLastReturnsVeryLastMatch)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("aa")) , FString(TEXT("aa")), FString(TEXT("aa")) };

    TOptional<FString&> Result = Ref(From).FindLast(FStringView(TEXT("aa")));

    return Result.GetPtrOrNull() == std::addressof(From[3]);
}

UR_TEST(UnrealRanges, Find, TestFindLastWithProjectionWhichReturnsRValue)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("a")), FString(TEXT("b")), FString(TEXT("b")) };

    TOptional<FString&> Result = Ref(From).FindLast(TEXT("bb"), [](const FString& Str) { return Str + Str; });

    return Result.GetPtrOrNull() == std::addressof(From[3]);
}

UR_TEST(UnrealRanges, Find, TestFindLastWithProjectionBeingMemberPtr)
{
    TArray<FString> From = { FString(TEXT("a")), FString(TEXT("aa")), FString(TEXT("bb")), FString(TEXT("b")) };

    TOptional<FString&> Result = Ref(From).FindLast(2, &FString::Len);

    return Result.GetPtrOrNull() == std::addressof(From[2]);
}

UR_TEST(UnrealRanges, Find, TestFindLastByPredicate)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("23")), FString(TEXT("23")), FString(TEXT("2333")) };

    TOptional<FString&> Result = Ref(From).FindLastBy([](FString& Str) { return Str == TEXT("23"); });

    return Result.GetPtrOrNull() == std::addressof(From[2]);
}

UR_TEST(UnrealRanges, Find, TestFindLastCalledOnConstRangeReturnsConstResult)
{
    TArray<FString> From = { FString(TEXT("1")), FString(TEXT("23")), FString(TEXT("233")), FString(TEXT("233")), FString(TEXT("23")) };
    const TRefView<TArray<FString>> RefView = Ref(From);

    TOptional<const FString&> Result0 = RefView.FindLast(TEXT("233"));
    TOptional<const FString&> Result1 = RefView.FindLast(3, &FString::Len);
    TOptional<const FString&> Result2 = RefView.FindLastBy([](const FString& Str) { return Str == TEXT("23"); });
    TOptional<const FString&> Result3 = RefView.FindLastOf(TArray<FString>{ FString(TEXT("2333")), FString(TEXT("233")) });

    return Result0.GetPtrOrNull() == std::addressof(From[3]) && Result1.GetPtrOrNull() == std::addressof(From[3]) && Result2.GetPtrOrNull() == std::addressof(From[4]) && Result3.GetPtrOrNull() == std::addressof(From[3]);
}

#endif