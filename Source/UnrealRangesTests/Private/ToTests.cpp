// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "UnrealRanges/Owned.h"
#include "EqualTo.h"
#include "Containers/Array.h"
#include "Containers/Set.h"
#include "Containers/Map.h"
#include "Containers/ChunkedArray.h"
#include "Containers/Deque.h"
#include "Containers/IndirectArray.h"
#include "Containers/SparseArray.h"
#include "Containers/UnrealString.h"
#include "Containers/List.h"
#include "Containers/StaticArray.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ToTArray, "UnrealRanges.To.TestToTArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ToTArray::RunTest(const FString& Parameters)
{
    TSet<int32> From({ 1, 2, 3 });

    TArray<int32> Result = Ur::View::Ref(From).To<TArray>();

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ToTSet, "UnrealRanges.To.TestToTSet", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ToTSet::RunTest(const FString& Parameters)
{
    TArray<int32> From({ 1, 2, 3 });

    TSet<int32> Result = Ur::View::Ref(From).To<TSet>();

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FromTMapToTMap, "UnrealRanges.To.TestFromTMapToTMap", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FromTMapToTMap::RunTest(const FString& Parameters)
{
    TMap<int32, int32> From = { {1,2}, {2,3} };

    TMap<int32, int32> Result = Ur::View::Ref(From).To<TMap>();

    return EqualTo(Result, { TPair<int32, int32>(1, 2), TPair<int32, int32>(2, 3) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FromMapLikeSequenceToTMap, "UnrealRanges.To.TestFromMapLikeSequenceToTMap", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FromMapLikeSequenceToTMap::RunTest(const FString& Parameters)
{
    TArray<TPair<int32, int32>> From = { TPair<int32, int32>{1,2}, TPair<int32, int32>{2,3} };

    TMap<int32, int32> Result = Ur::View::Ref(From).To<TMap>();

    return EqualTo(Result, { TPair<int32, int32>(1, 2), TPair<int32, int32>(2, 3) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FromMapLikeSequenceToTMapWithDuplicates, "UnrealRanges.To.TestFromMapLikeSequenceToTMapWithDuplicates", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FromMapLikeSequenceToTMapWithDuplicates::RunTest(const FString& Parameters)
{
    TArray<TPair<int32, int32>> From = { TPair<int32, int32>{1,2}, TPair<int32, int32>{1,3} };

    TMap<int32, int32> Result = Ur::View::Ref(From).To<TMap>();

    return EqualTo(Result, { TPair<int32, int32>(1, 3) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FromTMapToTMultiMap, "UnrealRanges.To.TestFromTMapToTMultiMap", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FromTMapToTMultiMap::RunTest(const FString& Parameters)
{
    TMultiMap<int32, int32> From = { {1,2}, {2,3} };

    TMultiMap<int32, int32> Result = Ur::View::Ref(From).To<TMultiMap>();

    return EqualTo(Result, { TPair<int32, int32>(1, 2), TPair<int32, int32>(2, 3) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FromMapLikeSequenceToTMultiMap, "UnrealRanges.To.TestFromMapLikeSequenceToTMultiMap", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FromMapLikeSequenceToTMultiMap::RunTest(const FString& Parameters)
{
    TArray<TPair<int32, int32>> From = { TPair<int32, int32>{1,2}, TPair<int32, int32>{2,3} };

    TMultiMap<int32, int32> Result = Ur::View::Ref(From).To<TMultiMap>();

    return EqualTo(Result, { TPair<int32, int32>(1, 2), TPair<int32, int32>(2, 3) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FromMapLikeSequenceToTMultiMapWithDuplicates, "UnrealRanges.To.TestFromMapLikeSequenceToTMultiMapWithDuplicates", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FromMapLikeSequenceToTMultiMapWithDuplicates::RunTest(const FString& Parameters)
{
    TArray<TPair<int32, int32>> From = { TPair<int32, int32>{1,2}, TPair<int32, int32>{1,3} };

    TMultiMap<int32, int32> Result = Ur::View::Ref(From).To<TMultiMap>();

    return EqualTo(Result, { TPair<int32, int32>{1,2}, TPair<int32, int32>(1, 3) });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ToTDeque, "UnrealRanges.To.TestToTDeque", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ToTDeque::RunTest(const FString& Parameters)
{
    TArray<int32> From({ 1, 2, 3 });

    TDeque<int32> Result = Ur::View::Ref(From).To<TDeque>();

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ToTIndirectArray, "UnrealRanges.To.TestToTIndirectArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ToTIndirectArray::RunTest(const FString& Parameters)
{
    TArray<Ur::TOwned<int32*>> FromPtrs({ Ur::Owned(new int32(1)), Ur::Owned(new int32(2)), Ur::Owned(new int32(3)) });

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result = Ur::View::Ref(FromPtrs).To<TIndirectArray>();

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ToExactlyTIndirectArray, "UnrealRanges.To.TestToExactlyTIndirectArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ToExactlyTIndirectArray::RunTest(const FString& Parameters)
{
    TArray<Ur::TOwned<int32*>> FromPtrs({ Ur::Owned(new int32(1)), Ur::Owned(new int32(2)), Ur::Owned(new int32(3)) });

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result = Ur::View::Ref(FromPtrs).ToExactly<TIndirectArray<int32>>();

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_IntoTIndirectArray, "UnrealRanges.To.TestIntoTIndirectArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_IntoTIndirectArray::RunTest(const FString& Parameters)
{
    TArray<Ur::TOwned<int32*>> FromPtrs({ Ur::Owned(new int32(1)), Ur::Owned(new int32(2)), Ur::Owned(new int32(3)) });

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result;
    Ur::View::Ref(FromPtrs).Into(Result);

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_UniquePtrToTIndirectArray, "UnrealRanges.To.TestUniquePtrToTIndirectArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_UniquePtrToTIndirectArray::RunTest(const FString& Parameters)
{
    TArray<TUniquePtr<int32>> FromPtrs;
    FromPtrs.Add(MakeUnique<int32>(1));
    FromPtrs.Add(MakeUnique<int32>(2));
    FromPtrs.Add(MakeUnique<int32>(3));

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result = Ur::View::Ref(FromPtrs).To<TIndirectArray>();

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_UniquePtrToExactlyTIndirectArray, "UnrealRanges.To.TestUniquePtrToExactlyTIndirectArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_UniquePtrToExactlyTIndirectArray::RunTest(const FString& Parameters)
{
    TArray<TUniquePtr<int32>> FromPtrs;
    FromPtrs.Add(MakeUnique<int32>(1));
    FromPtrs.Add(MakeUnique<int32>(2));
    FromPtrs.Add(MakeUnique<int32>(3));

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result = Ur::View::Ref(FromPtrs).ToExactly<TIndirectArray<int32>>();

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_UniquePtrIntoTIndirectArray, "UnrealRanges.To.TestUniquePtrIntoTIndirectArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_UniquePtrIntoTIndirectArray::RunTest(const FString& Parameters)
{
    TArray<TUniquePtr<int32>> FromPtrs;
    FromPtrs.Add(MakeUnique<int32>(1));
    FromPtrs.Add(MakeUnique<int32>(2));
    FromPtrs.Add(MakeUnique<int32>(3));

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result;
    Ur::View::Ref(FromPtrs).Into(Result);

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ToTSparseArray, "UnrealRanges.To.TestToTSparseArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ToTSparseArray::RunTest(const FString& Parameters)
{
    TArray<int32> From({ 1, 2, 3 });

    TSparseArray<int32> Result = Ur::View::Ref(From).To<TSparseArray>();

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FromCharSeqToExactlyFString, "UnrealRanges.To.TestFromCharSeqToExactlyFString", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FromCharSeqToExactlyFString::RunTest(const FString& Parameters)
{
    TArray<TCHAR> From({ TEXT('A'), TEXT('B'), TEXT('C') });

    FString Result = Ur::View::Ref(From).ToExactly<FString>();

    return EqualTo(Result, FString(TEXT("ABC")));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FromIntSeqToExactlyFString, "UnrealRanges.To.TestFromIntSeqToExactlyFString", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FromIntSeqToExactlyFString::RunTest(const FString& Parameters)
{
    TArray<int32> From({ 1, 2, 3 });

    FString Result = Ur::View::Ref(From).ToExactly<FString>();

    return EqualTo(Result, FString(TEXT("123")));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_FromFStringSeqToExactlyFString, "UnrealRanges.To.TestFromFStringSeqToExactlyFString", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_FromFStringSeqToExactlyFString::RunTest(const FString& Parameters)
{
    TArray<FString> From({ FString(TEXT("123")), FString(TEXT("abc")) });

    FString Result = Ur::View::Ref(From).ToExactly<FString>();

    return EqualTo(Result, FString(TEXT("123abc")));
}




IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_ToExactlyTChunkedArray, "UnrealRanges.To.TestToExactlyTChunkedArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_ToExactlyTChunkedArray::RunTest(const FString& Parameters)
{
    TArray<int32> From({ 1, 2, 3 });

    // TChunkedArray have NTTP parameter thus To<TChunkedArray> isn't possible
    TChunkedArray<int32> Result = Ur::View::Ref(From).ToExactly<TChunkedArray<int32>>();

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_IntoTDoubleLinkedList, "UnrealRanges.To.TestIntoTDoubleLinkedList", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_IntoTDoubleLinkedList::RunTest(const FString& Parameters)
{
    TArray<int32> From({ 1, 2, 3 });

    // TDoubleLinkedList<int32> isn't movable so it should be created beforehand
    TDoubleLinkedList<int32> Result;
    Ur::View::Ref(From).Into(Result);

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_IntoTStaticArray, "UnrealRanges.To.TestIntoTStaticArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_IntoTStaticArray::RunTest(const FString& Parameters)
{
    TArray<int32> From({ 1, 2, 3 });

    // TStaticArray have NTTP parameter thus To<TStaticArray> isn't possible
    TStaticArray<int32, 3> Result;
    Ur::View::Ref(From).Into(Result);

    return EqualTo(Result, { 1, 2, 3 });
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_IntoTStaticArrayOverSize, "UnrealRanges.To.TestIntoTStaticArrayOverSize", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_IntoTStaticArrayOverSize::RunTest(const FString& Parameters)
{
    TArray<int32> From({ 1, 2, 3 });

    // TStaticArray have NTTP parameter thus To<TStaticArray> isn't possible
    TStaticArray<int32, 2> Result;
    Ur::View::Ref(From).Into(Result);

    return EqualTo(Result, { 1, 2 });
}

#endif