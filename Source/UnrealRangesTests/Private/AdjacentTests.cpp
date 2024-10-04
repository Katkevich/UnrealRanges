// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_AdjacentViewOverEmptyRange, "UnrealRanges.Adjacent.TestAdjacentViewOverEmptyRange", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_AdjacentViewOverEmptyRange::RunTest(const FString& Parameters)
{
    TArray<int32> From;

    auto Result = Ref(From).Adjacent();

    static_assert(std::is_same_v<TAdjacentView<TRefView<TArray<int32>>, 2>, decltype(Result)>);
    static_assert(std::is_same_v<TTuple<int32&, int32&>, decltype(*Result.begin())>);

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_AdjacentViewOverRangeWithTooLittleElements, "UnrealRanges.Adjacent.TestAdjacentViewOverRangeWithTooLittleElements", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_AdjacentViewOverRangeWithTooLittleElements::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2 };

    auto Result = Ref(From).Adjacent<3>();

    static_assert(std::is_same_v<TAdjacentView<TRefView<TArray<int32>>, 3>, decltype(Result)>);
    static_assert(std::is_same_v<TTuple<int32&, int32&, int32&>, decltype(*Result.begin())>);

    return Result.begin() == Result.end();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_AdjacentViewOverRangeWithElementsCountEqToAdjacentWindow, "UnrealRanges.Adjacent.TestAdjacentViewOverRangeWithElementsCountEqToAdjacentWindow", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_AdjacentViewOverRangeWithElementsCountEqToAdjacentWindow::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3 };

    auto Result = Ref(From).Adjacent<3>();

    static_assert(std::is_same_v<TAdjacentView<TRefView<TArray<int32>>, 3>, decltype(Result)>);
    static_assert(std::is_same_v<TTuple<int32&, int32&, int32&>, decltype(*Result.begin())>);

    int32 One = 1;
    int32 Two = 2;
    int32 Three = 3;

    return *Result.begin() == TTuple<int32&, int32&, int32&>(One, Two, Three);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_AdjacentViewOverRValues, "UnrealRanges.Adjacent.TestAdjacentViewOverRValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_AdjacentViewOverRValues::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5 };

    auto Result = Ref(From)
        .Transform([](int32 Num) { return Num; })
        .Adjacent<3>();

    static_assert(std::is_same_v<TTuple<int32, int32, int32>, decltype(*Result.begin())>);

    return EqualTo(Result, { MakeTuple(1,2,3), MakeTuple(2,3,4), MakeTuple(3,4,5) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_AdjacentViewOverFilterView, "UnrealRanges.Adjacent.TestAdjacentViewOverFilterView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_AdjacentViewOverFilterView::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5,6,7,8 };

    auto Result = Ref(From)
        .Filter(Ur::Fn::Even)
        .Adjacent<3>();

    std::array Nums = { 2,4,6,8 };

    return EqualTo(Result, { ForwardAsTuple(Nums[0], Nums[1], Nums[2]), ForwardAsTuple(Nums[1], Nums[2], Nums[3]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_AdjacentViewOverTakeView, "UnrealRanges.Adjacent.TestAdjacentViewOverTakeView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_AdjacentViewOverTakeView::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5,6,7,8 };

    auto Result = Ref(From)
        .Take(4)
        .Adjacent();

    std::array Nums = { 1,2,3,4 };

    return EqualTo(Result, { ForwardAsTuple(Nums[0], Nums[1]), ForwardAsTuple(Nums[1], Nums[2]), ForwardAsTuple(Nums[2], Nums[3]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_AdjacentViewInternalIteration, "UnrealRanges.Adjacent.TestAdjacentViewInternalIteration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_AdjacentViewInternalIteration::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5,6,7,8 };

    auto Result = Ref(From)
        .Drop(4)
        .Adjacent()
        .To<TArray>();

    std::array Nums = { 5,6,7,8 };

    return Result.Num() == 3 && EqualTo(Result, { ForwardAsTuple(Nums[0], Nums[1]), ForwardAsTuple(Nums[1], Nums[2]), ForwardAsTuple(Nums[2], Nums[3]) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_AdjacentViewSize, "UnrealRanges.Adjacent.TestAdjacentViewSize", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_AdjacentViewSize::RunTest(const FString& Parameters)
{
    TArray<int32> From = { 1,2,3,4,5,6,7,8 };

    auto Result = Ref(From)
        .Drop(4)
        .Adjacent();

    std::array Nums = { 5,6,7,8 };

    return Result.Num() == 3 && EqualTo(Result, { ForwardAsTuple(Nums[0], Nums[1]), ForwardAsTuple(Nums[1], Nums[2]), ForwardAsTuple(Nums[2], Nums[3]) });
}

#endif