// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Traits.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatOfInts, "UnrealRanges.Concat.TestConcatOfInts", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatOfInts::RunTest(const FString& Parameters)
{
    TDoubleLinkedList<int32> List;
    TArray<int32> Array = { 1,2,3 };
    TSet<int32> Set = { 4,5,6 };

    auto ConcatView = Concat(List, Array, Set);

    return EqualTo(ConcatView, { 1,2,3,4,5,6 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatOfIntsReverse, "UnrealRanges.Concat.TestConcatOfIntsReverse", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatOfIntsReverse::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };
    TArray<int32> Array2 = { 4,5,6 };

    auto ConcatView = Concat(Array, Array2).Reverse();

    return EqualTo(ConcatView, { 6,5,4,3,2,1 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatOfIntsWithEmptyRanges, "UnrealRanges.Concat.TestConcatOfIntsWithEmptyRanges", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatOfIntsWithEmptyRanges::RunTest(const FString& Parameters)
{
    TArray<int32> EmptyArray;
    TArray<int32> EmptyArray2;
    TArray<int32> Array = { 1,2,3 };
    TArray<int32> EmptyArray3;
    TArray<int32> Array2 = { 4,5,6 };
    TArray<int32> EmptyArray4;

    auto ConcatView = Concat(EmptyArray, EmptyArray2, Array, EmptyArray3, Array2, EmptyArray4).Reverse();

    return EqualTo(ConcatView, { 6,5,4,3,2,1 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatAllEmptyCheckIterators, "UnrealRanges.Concat.TestConcatAllEmptyCheckIterators", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatAllEmptyCheckIterators::RunTest(const FString& Parameters)
{
    TArray<int32> EmptyArray;
    TArray<int32> EmptyArray2;
    TArray<int32> EmptyArray3;
    TArray<int32> EmptyArray4;

    auto ConcatView = Concat(EmptyArray, EmptyArray2, EmptyArray3, EmptyArray4);

    return ConcatView.begin() == ConcatView.end() && ConcatView.rbegin() == ConcatView.rend();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatAllEmptyCheckInternalIteration, "UnrealRanges.Concat.TestConcatAllEmptyCheckInternalIteration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatAllEmptyCheckInternalIteration::RunTest(const FString& Parameters)
{
    TArray<int32> EmptyArray;
    TArray<int32> EmptyArray2;
    TArray<int32> EmptyArray3;
    TArray<int32> EmptyArray4;

    auto Result = Concat(EmptyArray, EmptyArray2, EmptyArray3, EmptyArray4)
        .To<TArray>();

    return EqualTo(Result, std::initializer_list<int32>{});
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatInternalIterationInterruptsInTheMiddle, "UnrealRanges.Concat.TestConcatInternalIterationInterruptsInTheMiddle", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatInternalIterationInterruptsInTheMiddle::RunTest(const FString& Parameters)
{
    TArray<int32> EmptyArray1;
    TArray<int32> Array = { 1,2,3 };
    TArray<int32> EmptyArray2;
    TArray<int32> Array2 = { 4,5,6 };
    TArray<int32> Array3 = { 7,8,9 };

    auto Result = Concat(EmptyArray1, Array, EmptyArray2, Array2, Array3)
        .Take(4)
        .To<TArray>();

    return EqualTo(Result, { 1,2,3,4 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatMixinWithAnotherView, "UnrealRanges.Concat.TestConcatMixinWithAnotherView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatMixinWithAnotherView::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };
    TArray<int32> Array2;
    TArray<int32> Array3 = { 7,8,9 };

    auto Result = Ref(Array).ConcatWith(Ref(Array2), Ref(Array3));

    static_assert(std::is_same_v<decltype(Result), TConcatView<TRefView<TArray<int32>>, TRefView<TArray<int32>>, TRefView<TArray<int32>>>>);

    return Result.Count() == 6 && EqualTo(Result, { 1,2,3,7,8,9 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatMixinWithContainers, "UnrealRanges.Concat.TestConcatMixinWithContainers", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatMixinWithContainers::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };
    TArray<int32> Array2;
    TArray<int32> Array3 = { 7,8,9 };

    auto Result = Ref(Array).ConcatWith(Array2, Array3);

    static_assert(std::is_same_v<Ur::RangeReference<decltype(Result)>, int32&>);
    static_assert(std::is_same_v<decltype(Result), TConcatView<TRefView<TArray<int32>>, TRefView<TArray<int32>>, TRefView<TArray<int32>>>>);

    return Result.Count() == 6 && EqualTo(Result, { 1,2,3,7,8,9 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatMixinWithContainersAndViewsMix, "UnrealRanges.Concat.TestConcatMixinWithContainersAndViewsMix", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatMixinWithContainersAndViewsMix::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };
    TArray<int32> Array2;
    TArray<int32> Array3 = { 7,8,9 };

    auto Result = Ref(Array).ConcatWith(Cref(Array2), Array3);

    static_assert(std::is_same_v<Ur::RangeReference<decltype(Result)>, const int32&>);
    static_assert(std::is_same_v<decltype(Result), TConcatView<TRefView<TArray<int32>>, TRefView<const TArray<int32>>, TRefView<TArray<int32>>>>);

    return Result.Count() == 6 && EqualTo(Result, { 1,2,3,7,8,9 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatRValuesLValuesMix, "UnrealRanges.Concat.TestConcatRValuesLValuesMix", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatRValuesLValuesMix::RunTest(const FString& Parameters)
{
    const TCHAR* Chars = TEXT("abcdef");

    TArray<int32> Array0 = { 1,2,3 };
    TArray<FString> Array1 = { FString(TEXT("xxx")) };
    TArray<int32> Array3 = { 2,3,4 };

    auto Result = Ref(Array0)
        .Transform([&](int32 Num) { return FString(Num, Chars); })
        .ConcatWith(
            Array1,
            Ref(Array3).Transform([&](int32 Num) { return FString(Num, Chars); }));

    static_assert(std::is_same_v<Ur::RangeReference<decltype(Result)>, FString>);

    return Result.Count() == 7 && EqualTo(Result, { FString(TEXT("a")), FString(TEXT("ab")) , FString(TEXT("abc")) , FString(TEXT("xxx")) , FString(TEXT("ab")) ,FString(TEXT("abc")) , FString(TEXT("abcd")) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatOnReverseView, "UnrealRanges.Concat.TestConcatOnReverseView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatOnReverseView::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };
    TArray<int32> Array1 = {5};
    TArray<int32> Array2;
    TArray<int32> Array3 = { 7,8,9 };

    auto Result = Ref(Array)
        .Reverse()
        .ConcatWith(Array1, Array2, Cref(Array3).Reverse());

    static_assert(std::is_same_v<Ur::RangeReference<decltype(Result)>, const int32&>);
    static_assert(std::is_same_v<decltype(Result), TConcatView<TReverseView<TRefView<TArray<int32>>>, TRefView<TArray<int32>>, TRefView<TArray<int32>>, TReverseView<TRefView<const TArray<int32>>>>>);

    auto ResultArray = Result.To<TArray>();

    return Result.Count() == 7 && ResultArray.Num() == 7 && EqualTo(Result, {3,2,1,5,9,8,7});
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ConcatDifferentValueTypesImplicitlyConvertible, "UnrealRanges.Concat.TestDifferentValueTypesImplicitlyConvertible", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ConcatDifferentValueTypesImplicitlyConvertible::RunTest(const FString& Parameters)
{
    {
        TArray<FString> Array;

        auto MixedConcat = Ref(Array)
            .ConcatWith(Single(TEXT("1")));
        static_assert(std::is_same_v<decltype(*MixedConcat.begin()), FString>);
    }
    {
        TArray<FString> Array;

        auto MixedConcat = Ref(Array)
            .ConcatWith(SingleRef(TEXT("1")));
        static_assert(std::is_same_v<decltype(*MixedConcat.begin()), FString>);
    }
    {
        TArray<int32> Array;

        auto MixedConcat = Ref(Array)
            .ConcatWith(Single(int64(1)));
        static_assert(std::is_same_v<decltype(*MixedConcat.begin()), int64>);
    }
    return true;
}


#endif