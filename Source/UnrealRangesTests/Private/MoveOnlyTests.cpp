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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MoveOnlyViewRValueSelfMove, "UnrealRanges.MoveOnly.TestRValueSelfMove", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MoveOnlyViewRValueSelfMove::RunTest(const FString& Parameters)
{
    auto MaybeView = Maybe(TOptional<FMoveOnly>{ FMoveOnly{} });
    (void)MaybeView;

    // won't compile coz trying to copy underlying MaybeView (which is move-only) view inside TransformView/FilterView etc.
    // 
    // MaybeView.Transform([](FMoveOnly& Val) {});
    // MaybeView.Filter([](FMoveOnly& Val) { return true; });
    // etc.

    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).Transform([](FMoveOnly& Val) { return MoveTemp(Val); });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).Filter([](FMoveOnly& Val) { return true; });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).Enumerate();
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).Reverse();
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).Take(5);
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).TakeWhile([](FMoveOnly& Val) { return true; });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).ConcatWith(MaybeVal(FMoveOnly{}));

    MoveTemp(MaybeView).ConcatWith(MaybeVal(FMoveOnly{}));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_MoveOnlyPredicatesAndProjections, "UnrealRanges.MoveOnly.TestMoveOnlyPredicatesAndProjections", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_MoveOnlyPredicatesAndProjections::RunTest(const FString& Parameters)
{
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).Transform([Member = FMoveOnly{}](FMoveOnly& Val) { return MoveTemp(Val); });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).Filter([Member = FMoveOnly{}](FMoveOnly& Val) { return true; });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).TakeWhile([Member = FMoveOnly{}](FMoveOnly& Val) { return true; });

    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).FindBy([Member = FMoveOnly{}](FMoveOnly& Val) { return true; });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).ContainsBy([Member = FMoveOnly{}](FMoveOnly& Val) { return true; });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).CountIf([Member = FMoveOnly{}](FMoveOnly& Val) { return true; });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).FoldLeft([Member = FMoveOnly{}](const FMoveOnly& Accum, FMoveOnly& Val) { return FMoveOnly{}; });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).FoldRight([Member = FMoveOnly{}](const FMoveOnly& Accum, FMoveOnly& Val) { return FMoveOnly{}; });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).Min([Member = FMoveOnly{}](FMoveOnly& Val) { return 0; });
    Maybe(TOptional<FMoveOnly>{ FMoveOnly{} }).Sum([Member = FMoveOnly{}](FMoveOnly& Val) { return 0; });

    return true;
}

#endif