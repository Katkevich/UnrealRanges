// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/Fn/All.h"
#include "UnrealRanges/Traits.h"
#include "Containers/Array.h"

using namespace Ur::Fn;

struct FVal
{};

UR_TEST(UnrealRanges, Functor, TestKeyFunctor)
{
    FVal Val;
    FString Str = TEXT("Str");
    const FString ConstStr = TEXT("ConstStr");

    TTuple<FString, FVal> UeTuple(TEXT("fst"), FVal{});
    const TTuple<FString, FVal> ConstUeTuple(TEXT("fst"), FVal{});
    TTuple<FString&, FVal&> UeTupleRef(Str, Val);
    const TTuple<FString&, FVal&> ConstUeTupleRef(Str, Val);
    TTuple<const FString&, const FVal&> UeTupleConstRef(ConstStr, Val);

    std::tuple<FString, FVal> StdTuple(TEXT("fst"), FVal{});
    const std::tuple<FString, FVal> ConstStdTuple(TEXT("fst"), FVal{});
    std::pair<FString, FVal> StdPair(TEXT("fst"), FVal{});
    const std::pair<FString, FVal> ConstStdPair(TEXT("fst"), FVal{});
    std::pair<FString&, FVal&> StdPairRef(Str, Val);
    const std::pair<FString&, FVal&> ConstStdPairRef(Str, Val);
    std::pair<const FString&, const FVal&> StdPairConstRef(Str, Val);

    static_assert(std::is_same_v<decltype(Key(UeTuple)), FString&>);
    static_assert(std::is_same_v<decltype(Key(ConstUeTuple)), const FString&>);
    static_assert(std::is_same_v<decltype(Key(UeTupleRef)), FString&>);
    static_assert(std::is_same_v<decltype(Key(ConstUeTupleRef)), FString&>);
    static_assert(std::is_same_v<decltype(Key(UeTupleConstRef)), const FString&>);

    static_assert(std::is_same_v<decltype(Key(StdTuple)), FString&>);
    static_assert(std::is_same_v<decltype(Key(ConstStdTuple)), const FString&>);
    static_assert(std::is_same_v<decltype(Key(StdPair)), FString&>);
    static_assert(std::is_same_v<decltype(Key(ConstStdPair)), const FString&>);
    static_assert(std::is_same_v<decltype(Key(StdPairRef)), FString&>);
    static_assert(std::is_same_v<decltype(Key(ConstStdPairRef)), FString&>);
    static_assert(std::is_same_v<decltype(Key(StdPairConstRef)), const FString&>);

    static_assert(std::is_same_v<decltype(Key(TTuple<FString, FString>(TEXT(""), TEXT("")))), FString>);
    static_assert(std::is_same_v<decltype(Key(TTuple<FString&, FString&>(Str, Str))), FString&>);
    static_assert(std::is_same_v<decltype(Key(TTuple<const FString&, const FString&>(Str, Str))), const FString&>);

    return true;
}

UR_TEST(UnrealRanges, Functor, TestValueFunctor)
{
    FVal Val;
    FString Str = TEXT("Str");
    const FString ConstStr = TEXT("ConstStr");

    TTuple<FString, FVal> UeTuple(TEXT("fst"), FVal{});
    const TTuple<FString, FVal> ConstUeTuple(TEXT("fst"), FVal{});
    TTuple<FString&, FVal&> UeTupleRef(Str, Val);
    const TTuple<FString&, FVal&> ConstUeTupleRef(Str, Val);
    TTuple<const FString&, const FVal&> UeTupleConstRef(ConstStr, Val);

    std::tuple<FString, FVal> StdTuple(TEXT("fst"), FVal{});
    const std::tuple<FString, FVal> ConstStdTuple(TEXT("fst"), FVal{});
    std::pair<FString, FVal> StdPair(TEXT("fst"), FVal{});
    const std::pair<FString, FVal> ConstStdPair(TEXT("fst"), FVal{});
    std::pair<FString&, FVal&> StdPairRef(Str, Val);
    const std::pair<FString&, FVal&> ConstStdPairRef(Str, Val);
    std::pair<const FString&, const FVal&> StdPairConstRef(Str, Val);

    static_assert(std::is_same_v<decltype(Value(UeTuple)), FVal&>);
    static_assert(std::is_same_v<decltype(Value(ConstUeTuple)), const FVal&>);
    static_assert(std::is_same_v<decltype(Value(UeTupleRef)), FVal&>);
    static_assert(std::is_same_v<decltype(Value(ConstUeTupleRef)), FVal&>);
    static_assert(std::is_same_v<decltype(Value(UeTupleConstRef)), const FVal&>);

    static_assert(std::is_same_v<decltype(Value(StdTuple)), FVal&>);
    static_assert(std::is_same_v<decltype(Value(ConstStdTuple)), const FVal&>);
    static_assert(std::is_same_v<decltype(Value(StdPair)), FVal&>);
    static_assert(std::is_same_v<decltype(Value(ConstStdPair)), const FVal&>);
    static_assert(std::is_same_v<decltype(Value(StdPairRef)), FVal&>);
    static_assert(std::is_same_v<decltype(Value(ConstStdPairRef)), FVal&>);
    static_assert(std::is_same_v<decltype(Value(StdPairConstRef)), const FVal&>);

    static_assert(std::is_same_v<decltype(Value(TTuple<FString, FVal>(TEXT(""), FVal{}))), FVal>);
    static_assert(std::is_same_v<decltype(Value(TTuple<FString&, FVal&>(Str, Val))), FVal&>);
    static_assert(std::is_same_v<decltype(Value(TTuple<const FString&, const FVal&>(Str, Val))), const FVal&>);

    return true;
}


#endif