// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Traits.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

struct FMoveOnly
{
    FMoveOnly() = default;
    FMoveOnly(FMoveOnly&&) = default;

    FMoveOnly(const FMoveOnly&) = delete;
    FMoveOnly& operator=(const FMoveOnly&) = delete;
    FMoveOnly& operator=(FMoveOnly&&) = delete;

    int Num = 0;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MoveOnlyViewTestConcat, "UnrealRanges.MoveOnlyView.TestConcat", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MoveOnlyViewTestConcat::RunTest(const FString& Parameters)
{
    const TArray<FMoveOnly> MoveOnlyArray;

    auto Result = Ref(MoveOnlyArray)
        .Filter([](const FMoveOnly& Obj) { return Obj.Num == 0; });

    static_assert(std::is_same_v<decltype(*Result.begin()), const FMoveOnly&>);
    //auto MoveOnlyView = Ref(MoveOnlyArray).ConcatWith(MaybeVal(FMoveOnly{}));
    //MoveOnlyView.ConcatWith(MaybeVal(FMoveOnly{}));

    //Ref(MoveOnlyArray).ConcatWith(MaybeVal(FMoveOnly{})).ConcatWith(MaybeVal(FMoveOnly{}));
    //Ref(MoveOnlyArray).ConcatWith(MaybeVal(FMoveOnly{})).Enumerate();

    return false;
}

#endif