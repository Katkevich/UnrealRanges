// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Traits.h"
#include "Containers/Array.h"
#include "EqualTo.h"
#include <map>

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_UeTMapValues, "UnrealRanges.Values.TestUeTMapValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_UeTMapValues::RunTest(const FString& Parameters)
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FreeValuesFunction, "UnrealRanges.Values.TestFreeValuesFunction", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FreeValuesFunction::RunTest(const FString& Parameters)
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