// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Traits.h"
#include "Containers/Array.h"
#include "EqualTo.h"
#include <map>

using namespace Ur::View;

UR_TEST(UnrealRanges, Keys, TestUeTMapKeys)
{
    TMap<FString, FString> UeMap = { { TEXT("Key"), TEXT("Value") } };
    std::map<FString, FString> StdMap = { { TEXT("Key"), TEXT("Value") } };

    auto UeResult = Ref(UeMap).Keys();
    auto StdResult = Ref(StdMap).Keys();
    auto RValueResult = Ref(UeMap).Transform([](const auto& Kvp) { return Kvp; }).Keys();

    static_assert(std::is_same_v<decltype(UeResult)::reference, FString&>);
    static_assert(std::is_same_v<decltype(StdResult)::reference, const FString&>);
    static_assert(std::is_same_v<decltype(RValueResult)::reference, FString>);

    return
        EqualTo(UeResult, { TEXT("Key") }) &&
        EqualTo(StdResult, { TEXT("Key") }) &&
        EqualTo(RValueResult, { TEXT("Key") });
}

UR_TEST(UnrealRanges, Keys, TestFreeKeysFunction)
{
    TMap<FString, FString> UeMap = { { TEXT("Key"), TEXT("Value") } };
    std::map<FString, FString> StdMap = { { TEXT("Key"), TEXT("Value") } };

    auto UeResult = Keys(UeMap);
    auto StdResult = Keys(StdMap);

    static_assert(std::is_same_v<decltype(UeResult)::reference, FString&>);
    static_assert(std::is_same_v<decltype(StdResult)::reference, const FString&>);

    return
        EqualTo(UeResult, { TEXT("Key") }) &&
        EqualTo(StdResult, { TEXT("Key") });
}


#endif