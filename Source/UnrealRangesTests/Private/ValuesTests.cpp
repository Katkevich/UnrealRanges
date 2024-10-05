// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Traits.h"
#include "Containers/Array.h"
#include "EqualTo.h"
#include <map>

using namespace Ur::View;

UR_TEST(UnrealRanges, Values, TestUeTMapValues)
{
    TMap<FString, FString> UeMap = { { TEXT("Key"), TEXT("Value") } };
    std::map<FString, FString> StdMap = { { TEXT("Key"), TEXT("Value") } };

    auto UeResult = Ref(UeMap).Values();
    auto StdResult = Ref(StdMap).Values();
    auto RValueResult = Ref(UeMap).Transform([](const auto& Kvp) { return Kvp; }).Values();

    static_assert(std::is_same_v<decltype(UeResult)::reference, FString&>);
    static_assert(std::is_same_v<decltype(StdResult)::reference, FString&>);
    static_assert(std::is_same_v<decltype(RValueResult)::reference, FString>);

    return
        EqualTo(UeResult, { TEXT("Value") }) &&
        EqualTo(StdResult, { TEXT("Value") }) &&
        EqualTo(RValueResult, { TEXT("Value") });
}

UR_TEST(UnrealRanges, Values, TestFreeValuesFunction)
{
    TMap<FString, FString> UeMap = { { TEXT("Key"), TEXT("Value") } };
    std::map<FString, FString> StdMap = { { TEXT("Key"), TEXT("Value") } };

    auto UeResult = Values(UeMap);
    auto StdResult = Values(StdMap);

    static_assert(std::is_same_v<decltype(UeResult)::reference, FString&>);
    static_assert(std::is_same_v<decltype(StdResult)::reference, FString&>);

    return
        EqualTo(UeResult, { TEXT("Value") }) &&
        EqualTo(StdResult, { TEXT("Value") });
}

#endif