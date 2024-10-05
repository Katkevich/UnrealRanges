// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Fn/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;


UR_TEST(UnrealRanges, FoldLeftNoInit, TestRefView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).FoldLeft(Ur::Fn::Plus) == 6;
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestEnumerateView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Enumerate().FoldLeft([](FIndexed<int32> Lhs, FIndexed<int32> Rhs) { return FIndexed<int32>(Lhs.Value + Rhs.Value, 0); }).GetValue().Value == 6;
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestTransformVIew)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Transform([](int32 Num) { return Num; }).FoldLeft(Ur::Fn::Plus) == 6;
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestReverseView)
{
    TArray<int32> Array = { 2,2,3 };

    return Ref(Array).Reverse().FoldLeft(Ur::Fn::Mul) == 12;
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestTakeViewGreaterThanArray)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(10).FoldLeft(Ur::Fn::Plus) == 6;
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestTakeViewLessThanArray)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(2).FoldLeft(Ur::Fn::Plus) == 3;
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestFilterView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Filter([](int32 Num) { return Num % 2 == 1; }).FoldLeft(Ur::Fn::Plus) == 4;
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestTakeWhileView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).TakeWhile([](int32 Num) { return Num <= 2; }).FoldLeft(Ur::Fn::Plus) == 3;
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestLValueItemIntoRValueArgument)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    return Ref(Array).FoldLeft([](FString Lhs, FString Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("123"));
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestLValueItemIntoRValueLValueArgumentMix)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    return Ref(Array).FoldLeft([](FString Lhs, FString& Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("123"));
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestLValueItemIntoLValueRValueArgumentMix)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    // won't compile coz FString (result of Fn(...) which is an rvalue is being passed into Fn as a first argument) cannot be bound to non const lvalue reference
    //return Ref(Array).FoldLeft([](FString& Lhs, FString Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("123"));
    return true;
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestLValueItemIntoConstLValueRValueArgumentMix)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    // compiles coz FString can be bound to const lvalue reference
    return Ref(Array).FoldLeft([](const FString& Lhs, FString Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("123"));
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestRValueItemIntoRValueArg)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    // compiles coz FString can be bound to const lvalue reference
    return Ref(Array)
        .Transform([](const FString& Str) { return Str; })
        .FoldLeft([](FString&& Lhs, FString&& Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("123"));
}

UR_TEST(UnrealRanges, FoldLeftNoInit, TestToLittleElements)
{
    TArray<FString> Array = { FString(TEXT("1")) };

    return !Ref(Array).FoldLeft([](const FString& Lhs, const FString& Rhs) { return Lhs + Rhs; }).IsSet();
}




UR_TEST(UnrealRanges, FoldLeftWithInit, TestRefView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).FoldLeft(5, Ur::Fn::Plus) == 11;
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestEnumerateView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Enumerate().FoldLeft(FIndexed<int32>(5, 0), [](FIndexed<int32> Lhs, FIndexed<int32> Rhs) { return FIndexed<int32>(Lhs.Value + Rhs.Value, 0); }).Value == 11;
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestTransformVIew)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Transform([](int32 Num) { return Num; }).FoldLeft(5, Ur::Fn::Plus) == 11;
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestReverseView)
{
    TArray<int32> Array = { 2,2,3 };

    return Ref(Array).Reverse().FoldLeft(5, Ur::Fn::Mul) == 60;
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestTakeViewGreaterThanArray)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(10).FoldLeft(5, Ur::Fn::Plus) == 11;
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestTakeViewLessThanArray)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Take(2).FoldLeft(5, Ur::Fn::Plus) == 8;
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestFilterView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).Filter([](int32 Num) { return Num % 2 == 1; }).FoldLeft(5, Ur::Fn::Plus) == 9;
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestTakeWhileView)
{
    TArray<int32> Array = { 1,2,3 };

    return Ref(Array).TakeWhile([](int32 Num) { return Num <= 2; }).FoldLeft(5, Ur::Fn::Plus) == 8;
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestLValueItemIntoRValueArgument)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    FString Init = TEXT("init");
    return Ref(Array).FoldLeft(Init, [](FString Lhs, FString Rhs) { return Lhs + Rhs; }) == FString(TEXT("init123"));
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestLValueItemIntoRValueLValueArgumentMix)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    FString Init = TEXT("init");
    return Ref(Array).FoldLeft(Init, [](FString Lhs, FString& Rhs) { return Lhs + Rhs; }) == FString(TEXT("init123"));
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestLValueItemIntoLValueRValueArgumentMix)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    // won't compile coz init FString cannot be bound to non const lvalue reference
    //return Ref(Array).FoldLeft(FString(TEXT("init")), [](FString& Lhs, FString Rhs) { return Lhs + Rhs; }).GetValue() == FString(TEXT("init123"));
    return true;
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestLValueItemIntoConstLValueRValueArgumentMix)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("2")), FString(TEXT("3")) };

    // compiles coz FString can be bound to const lvalue reference
    return Ref(Array).FoldLeft(FString(TEXT("init")), [](const FString& Lhs, FString Rhs) { return Lhs + Rhs; }) == FString(TEXT("init123"));
}

UR_TEST(UnrealRanges, FoldLeftWithInit, TestInitHasDifferentType)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("22")), FString(TEXT("333")) };

    return Ref(Array).FoldLeft(5, [](int32 Accum, FString Rhs) { return Accum + Rhs.Len(); }) == 11;
}

UR_TEST(UnrealRanges, FoldRightNoInit, TestFoldRightNoInit)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("22")), FString(TEXT("333")) };

    return Ref(Array).FoldRight([](FString Lhs, FString Rhs) { return Lhs + Rhs; }) == FString(TEXT("333221"));
}

UR_TEST(UnrealRanges, FoldRightWithInit, TestFoldRightWithInit)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("22")), FString(TEXT("333")) };

    return Ref(Array).FoldRight(FString(TEXT("init")), [](FString Lhs, FString Rhs) { return Lhs + Rhs; }) == FString(TEXT("init333221"));
}

UR_TEST(UnrealRanges, FoldrightWithInit, TestReverseFoldRightIsEquivalentToFoldLeft)
{
    TArray<FString> Array = { FString(TEXT("1")), FString(TEXT("22")), FString(TEXT("333")) };

    return Ref(Array).Reverse().FoldRight(FString(TEXT("init")), [](FString Lhs, FString Rhs) { return Lhs + Rhs; }) == FString(TEXT("init122333"));
}

#endif