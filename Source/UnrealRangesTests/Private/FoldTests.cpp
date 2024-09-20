// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Fn/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_RefViewFoldLeft, "UnrealRanges.FoldLeft.NoInit.TestRefView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_RefViewFoldLeft::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).FoldLeft(Ur::Fn::Plus) == 6;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EnumerateViewFoldLeft, "UnrealRanges.FoldLeft.NoInit.TestEnumerateView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EnumerateViewFoldLeft::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Enumerate().FoldLeft([](FIndexed<int32> Lhs, FIndexed<int32> Rhs) { return FIndexed<int32>(Lhs.Value + Rhs.Value, 0); }).GetValue().Value == 6;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TransformViewFoldLeft, "UnrealRanges.FoldLeft.NoInit.TestTransformVIew", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TransformViewFoldLeft::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Transform([](int32 Num) { return Num; }).FoldLeft(Ur::Fn::Plus) == 6;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ReverseViewFoldLeft, "UnrealRanges.FoldLeft.NoInit.TestReverseView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ReverseViewFoldLeft::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 2,2,3 };

    return Ref(Array).Reverse().FoldLeft(Ur::Fn::Mul) == 12;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeViewFoldLeftGreaterThanArray, "UnrealRanges.FoldLeft.NoInit.TestTakeViewGreaterThanArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeViewFoldLeftGreaterThanArray::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(10).FoldLeft(Ur::Fn::Plus) == 6;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeViewFoldLeftLessThanArray, "UnrealRanges.FoldLeft.NoInit.TestTakeViewLessThanArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeViewFoldLeftLessThanArray::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(2).FoldLeft(Ur::Fn::Plus) == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FilterViewFoldLeft, "UnrealRanges.FoldLeft.NoInit.TestFilterView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FilterViewFoldLeft::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Filter([](int32 Num) { return Num % 2 == 1; }).FoldLeft(Ur::Fn::Plus) == 4;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeWhileViewFoldLeft, "UnrealRanges.FoldLeft.NoInit.TestTakeWhileView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeWhileViewFoldLeft::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).TakeWhile([](int32 Num) { return Num <= 2; }).FoldLeft(Ur::Fn::Plus) == 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_LValueItemIntoRValueArgument, "UnrealRanges.FoldLeft.NoInit.TestLValueItemIntoRValueArgument", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_LValueItemIntoRValueArgument::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    return Ref(Array).FoldLeft([](FString Lhs, FString Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("123"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_LValueItemIntoRValueLValueArgumentMix, "UnrealRanges.FoldLeft.NoInit.TestLValueItemIntoRValueLValueArgumentMix", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_LValueItemIntoRValueLValueArgumentMix::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    return Ref(Array).FoldLeft([](FString Lhs, FString& Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("123"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_LValueItemIntoLValueRValueArgumentMix, "UnrealRanges.FoldLeft.NoInit.TestLValueItemIntoLValueRValueArgumentMix", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_LValueItemIntoLValueRValueArgumentMix::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    // won't compile coz FString (result of Fn(...) which is an rvalue is being passed into Fn as a first argument) cannot be bound to non const lvalue reference
    //return Ref(Array).FoldLeft([](FString& Lhs, FString Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("123"));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_LValueItemIntoConstLValueRValueArgumentMix, "UnrealRanges.FoldLeft.NoInit.TestLValueItemIntoConstLValueRValueArgumentMix", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_LValueItemIntoConstLValueRValueArgumentMix::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    // compiles coz FString can be bound to const lvalue reference
    return Ref(Array).FoldLeft([](const FString& Lhs, FString Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("123"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FoldRValueItemIntoRValueArg, "UnrealRanges.FoldLeft.NoInit.TestRValueItemIntoRValueArg", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FoldRValueItemIntoRValueArg::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    // compiles coz FString can be bound to const lvalue reference
    return Ref(Array)
        .Transform([](const FString& Str) { return Str; })
        .FoldLeft([](FString&& Lhs, FString&& Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("123"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ToLittleElements, "UnrealRanges.FoldLeft.NoInit.TestToLittleElements", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ToLittleElements::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")) };

    return !Ref(Array).FoldLeft([](const FString& Lhs, const FString& Rhs) { return Lhs + Rhs; }).IsSet();
}




IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_RefViewFoldLeftWithInit, "UnrealRanges.FoldLeft.WithInit.TestRefView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_RefViewFoldLeftWithInit::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).FoldLeft(5, Ur::Fn::Plus) == 11;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_EnumerateViewFoldLeftWithInit, "UnrealRanges.FoldLeft.WithInit.TestEnumerateView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_EnumerateViewFoldLeftWithInit::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Enumerate().FoldLeft(FIndexed<int32>(5, 0), [](FIndexed<int32> Lhs, FIndexed<int32> Rhs) { return FIndexed<int32>(Lhs.Value + Rhs.Value, 0); }).Value == 11;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TransformViewFoldLeftWithInit, "UnrealRanges.FoldLeft.WithInit.TestTransformVIew", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TransformViewFoldLeftWithInit::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Transform([](int32 Num) { return Num; }).FoldLeft(5, Ur::Fn::Plus) == 11;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ReverseViewFoldLeftWithInit, "UnrealRanges.FoldLeft.WithInit.TestReverseView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ReverseViewFoldLeftWithInit::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 2,2,3 };

    return Ref(Array).Reverse().FoldLeft(5, Ur::Fn::Mul) == 60;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeViewFoldLeftWithInitGreaterThanArray, "UnrealRanges.FoldLeft.WithInit.TestTakeViewGreaterThanArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeViewFoldLeftWithInitGreaterThanArray::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(10).FoldLeft(5, Ur::Fn::Plus) == 11;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeViewFoldLeftWithInitLessThanArray, "UnrealRanges.FoldLeft.WithInit.TestTakeViewLessThanArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeViewFoldLeftWithInitLessThanArray::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(2).FoldLeft(5, Ur::Fn::Plus) == 8;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FilterViewFoldLeftWithInit, "UnrealRanges.FoldLeft.WithInit.TestFilterView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FilterViewFoldLeftWithInit::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Filter([](int32 Num) { return Num % 2 == 1; }).FoldLeft(5, Ur::Fn::Plus) == 9;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_TakeWhileViewFoldLeftWithInit, "UnrealRanges.FoldLeft.WithInit.TestTakeWhileView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_TakeWhileViewFoldLeftWithInit::RunTest(const FString& Parameters)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).TakeWhile([](int32 Num) { return Num <= 2; }).FoldLeft(5, Ur::Fn::Plus) == 8;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FoldLeftWithInitLValueItemIntoRValueArgument, "UnrealRanges.FoldLeft.WithInit.TestLValueItemIntoRValueArgument", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FoldLeftWithInitLValueItemIntoRValueArgument::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    FString Init = TEXT("init");
    return Ref(Array).FoldLeft(Init, [](FString Lhs, FString Rhs) { return Lhs + Rhs; }) == FString(TEXT("init123"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FoldLeftWithInitLValueItemIntoRValueLValueArgumentMix, "UnrealRanges.FoldLeft.WithInit.TestLValueItemIntoRValueLValueArgumentMix", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FoldLeftWithInitLValueItemIntoRValueLValueArgumentMix::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    FString Init = TEXT("init");
    return Ref(Array).FoldLeft(Init, [](FString Lhs, FString& Rhs) { return Lhs + Rhs; }) == FString(TEXT("init123"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FoldLeftWithInitLValueItemIntoLValueRValueArgumentMix, "UnrealRanges.FoldLeft.WithInit.TestLValueItemIntoLValueRValueArgumentMix", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FoldLeftWithInitLValueItemIntoLValueRValueArgumentMix::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    // won't compile coz init FString cannot be bound to non const lvalue reference
    //return Ref(Array).FoldLeft(FString(TEXT("init")), [](FString& Lhs, FString Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("init123"));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FoldLeftWithInitLValueItemIntoConstLValueRValueArgumentMix, "UnrealRanges.FoldLeft.WithInit.TestLValueItemIntoConstLValueRValueArgumentMix", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FoldLeftWithInitLValueItemIntoConstLValueRValueArgumentMix::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    // compiles coz FString can be bound to const lvalue reference
    return Ref(Array).FoldLeft(FString(TEXT("init")), [](const FString& Lhs, FString Rhs) { return Lhs + Rhs; }) == FString(TEXT("init123"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_InitHasDifferentType, "UnrealRanges.FoldLeft.WithInit.TestInitHasDifferentType", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_InitHasDifferentType::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("22")), FString(TEXT("333")) };

    return Ref(Array).FoldLeft(5, [](int32 Accum, FString Rhs) { return Accum + Rhs.Len(); }) == 11;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FoldRightNoInit, "UnrealRanges.FoldRight.NoInit.TestFoldRightNoInit", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FoldRightNoInit::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("22")), FString(TEXT("333")) };

    return Ref(Array).FoldRight([](FString Lhs, FString Rhs) { return Lhs + Rhs; }) == FString(TEXT("333221"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FoldRightWithInit, "UnrealRanges.FoldRight.WithInit.TestFoldRightWithInit", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FoldRightWithInit::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("22")), FString(TEXT("333")) };

    return Ref(Array).FoldRight(FString(TEXT("init")), [](FString Lhs, FString Rhs) { return Lhs + Rhs; }) == FString(TEXT("init333221"));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ReverseFoldRightIsEquivalentToFoldLeft, "UnrealRanges.Foldright.WithInit.TestReverseFoldRightIsEquivalentToFoldLeft", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ReverseFoldRightIsEquivalentToFoldLeft::RunTest(const FString& Parameters)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("22")), FString(TEXT("333")) };

    return Ref(Array).Reverse().FoldRight(FString(TEXT("init")), [](FString Lhs, FString Rhs) { return Lhs + Rhs; }) == FString(TEXT("init122333"));
}

#endif