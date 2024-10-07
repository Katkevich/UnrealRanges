// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Swizzle, SwizzleEmptyRangeOfLValueTuplesOfLValues)
{
    using FromType = TArray<TTuple<int32&, FString&, float&>>;
    FromType From;

    auto Result = Ref(From).Swizzle<1, 0, 2>();

    static_assert(std::is_same_v<TRefView<FromType>::reference, TTuple<int32&, FString&, float&>&>);
    static_assert(std::is_same_v<TSwizzleView<TRefView<FromType>, 1, 0, 2>, decltype(Result)>);
    static_assert(std::is_same_v<TSwizzleView<TRefView<FromType>, 1, 0, 2>::reference, TTuple<FString&, int32&, float&>>);
    static_assert(std::is_same_v<decltype(*Result.begin()), TTuple<FString&, int32&, float&>>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Swizzle, SwizzleEmptyRangeOfLValueTuplesOfRValues)
{
    using FromType = TArray<TTuple<int32, FString, float>>;
    FromType From;

    auto Result = Ref(From).Swizzle<2, 0, 1>();

    static_assert(std::is_same_v<TRefView<FromType>::reference, TTuple<int32, FString, float>&>);
    static_assert(std::is_same_v<TSwizzleView<TRefView<FromType>, 2, 0, 1>, decltype(Result)>);
    static_assert(std::is_same_v<TSwizzleView<TRefView<FromType>, 2, 0, 1>::reference, TTuple<float&, int32&, FString&>>);
    static_assert(std::is_same_v<decltype(*Result.begin()), TTuple<float&, int32&, FString&>>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Swizzle, SwizzleEmptyRangeOfRValueTuplesOfRValues)
{
    using FromType = TArray<TTuple<int32, FString, float>>;
    FromType From;

    auto Result = Ref(From)
        .Transform([](const auto& Tpl) { return Tpl; })
        .Swizzle<2, 0, 1>();

    static_assert(std::is_same_v<TRefView<FromType>::reference, TTuple<int32, FString, float>&>);
    static_assert(std::is_same_v<decltype(*Result.begin()), TTuple<float, int32, FString>>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Swizzle, SwizzleEmptyRangeOfRValueTuplesOfLValues)
{
    using FromType = TArray<TTuple<int32, FString, float&>>;
    FromType From;

    auto Result = Ref(From)
        .Transform([](const auto& Tpl) { return Tpl; })
        .Swizzle<2, 0, 1>();

    static_assert(std::is_same_v<TRefView<FromType>::reference, TTuple<int32, FString, float&>&>);
    static_assert(std::is_same_v<decltype(*Result.begin()), TTuple<float&, int32, FString>>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Swizzle, SwizzleEmptyRangeOfStdTuples)
{
    using FromType = TArray<std::tuple<int32&, FString, float>>;
    FromType From;

    auto Result = Ref(From).Swizzle<2, 0, 1>();

    static_assert(std::is_same_v<TRefView<FromType>::reference, std::tuple<int32&, FString, float>&>);
    static_assert(std::is_same_v<TSwizzleView<TRefView<FromType>, 2, 0, 1>, decltype(Result)>);
    static_assert(std::is_same_v<TSwizzleView<TRefView<FromType>, 2, 0, 1>::reference, TTuple<float&, int32&, FString&>>);

    return Result.begin() == Result.end();
}

UR_TEST(UnrealRanges, Swizzle, SwizzleNonEmptyRangeOfLValues)
{
    int32 Ints[] = { 1,2,3 };
    FString Strs[] = { TEXT("1"), TEXT("2"), TEXT("3") };
    float Fps[] = { 10.f, 20.f, 30.f };

    using FromType = TArray<TTuple<int32&, FString&, float&>>;
    FromType From = { ForwardAsTuple(Ints[0], Strs[0], Fps[0]), ForwardAsTuple(Ints[1], Strs[1], Fps[1]), ForwardAsTuple(Ints[2], Strs[2], Fps[2]) };

    auto Result = Ref(From).Swizzle<2, 0, 1>();

    return EqualTo(Result, { ForwardAsTuple(Fps[0], Ints[0], Strs[0]), ForwardAsTuple(Fps[1], Ints[1], Strs[1]), ForwardAsTuple(Fps[2], Ints[2], Strs[2]) });
}

UR_TEST(UnrealRanges, Swizzle, SwizzleNonEmptyRangeOfMixedLValuesAndRValues)
{
    int32 Ints[] = { 1,2,3 };
    FString Strs[] = { TEXT("1"), TEXT("2"), TEXT("3") };
    float Fps[] = { 10.f, 20.f, 30.f };

    using FromType = TArray<TTuple<int32&, FString, float&>>;
    FromType From = { ForwardAsTuple(Ints[0], Strs[0], Fps[0]), ForwardAsTuple(Ints[1], Strs[1], Fps[1]), ForwardAsTuple(Ints[2], Strs[2], Fps[2]) };

    auto Result = Ref(From).Swizzle<2, 0, 1>();

    using ResultItemType = TTuple<float&, int32&, FString&>;
    static_assert(std::is_same_v<ResultItemType, decltype(*Result.begin())>);

    return EqualTo(Result, { ResultItemType(Fps[0], Ints[0], Strs[0]), ResultItemType(Fps[1], Ints[1], Strs[1]), ResultItemType(Fps[2], Ints[2], Strs[2]) });
}

UR_TEST(UnrealRanges, Swizzle, SwizzleNonEmptyRangeOfRValueTuplesOfLValues)
{
    float Flt = 10.f;
    FString Str = TEXT("str");

    using FromType = TArray<TTuple<int32, FString, FString&, float&>>;
    FromType From = { { 1, TEXT("1"), Str, Flt }};

    auto Result = Ref(From)
        .Transform([](const auto& Tpl) { return Tpl; })
        .Swizzle<3, 2, 0, 1>();

    static_assert(std::is_same_v<decltype(*Result.begin()), TTuple<float&, FString&, int32, FString>>);

    auto Fst = Result.Front();

    return
        std::addressof(get<0>(*Fst)) == std::addressof(Flt) &&
        std::addressof(get<1>(*Fst)) == std::addressof(Str) &&
        get<2>(*Fst) == 1 &&
        get<3>(*Fst) == FString(TEXT("1"));
}

struct Aggregate
{
    int32 Int{};
    FString Str{};
};

template<>
struct std::tuple_size<Aggregate>
{
    static constexpr std::size_t value = 2;
};

template<std::size_t Idx>
struct std::tuple_element<Idx, Aggregate>
{
    using type = std::conditional_t<Idx == 0, int32, FString>;
};

template<std::size_t Idx>
decltype(auto) get(Aggregate& Value)
{
    if constexpr (Idx == 0)
        return (Value.Int);
    else
        return (Value.Str);
}

template<std::size_t Idx>
decltype(auto) get(const Aggregate& Value)
{
    if constexpr (Idx == 0)
        return (Value.Int);
    else
        return (Value.Str);
}


UR_TEST(UnrealRanges, Swizzle, SwizzleLValueAggregateWithTupleProtocol)
{
    using FromType = TArray<Aggregate>;
    FromType From = { Aggregate{ 1,TEXT("1") }, Aggregate{ 2,TEXT("2") } };

    auto Result = Ref(From)
        .Swizzle<1, 0>();

    static_assert(std::is_same_v<decltype(*Result.begin()), TTuple<FString&, int32&>>);

    return Result.Front() == TTuple<FString&, int32&>{ From[0].Str, From[0].Int };
}

UR_TEST(UnrealRanges, Swizzle, SwizzleRValueAggregateWithTupleProtocol)
{
    using FromType = TArray<Aggregate>;
    FromType From = { {1,TEXT("1")}, {2,TEXT("2")} };

    auto Result = Ref(From)
        .Transform([](const auto& Aggr) { return Aggr; })
        .Swizzle<1, 0>();

    static_assert(std::is_same_v<decltype(*Result.begin()), TTuple<FString, int32>>);

    return Result.Front() == TTuple<FString, int32>{ From[0].Str, From[0].Int };
}

UR_TEST(UnrealRanges, Swizzle, SwizzleOfAPairSupportsMapMixin)
{
    using FromType = TArray<Aggregate>;
    FromType From = { Aggregate{ 1,TEXT("1") }, Aggregate{ 2,TEXT("2") } };

    auto Result = Ref(From)
        .Swizzle<1, 0>()
        .Keys()
        .To<TArray>();

    static_assert(std::is_same_v<decltype(*Result.begin()), FString&>);

    return Result.Last() == FString(TEXT("2"));
}

#endif