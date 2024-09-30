// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Traits.h"
#include "Containers/Array.h"
#include "EqualTo.h"
#include <map>

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_UeTMapKeys, "UnrealRanges.Keys.TestUeTMapKeys", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_UeTMapKeys::RunTest(const FString& Parameters)
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FreeKeysFunction, "UnrealRanges.Keys.TestFreeKeysFunction", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FreeKeysFunction::RunTest(const FString& Parameters)
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