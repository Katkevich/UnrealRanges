// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

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

UR_TEST(UnrealRanges, To, TestToTArray)
{
    TSet<int32> From({ 1, 2, 3 });

    TArray<int32> Result = Ur::View::Ref(From).To<TArray>();

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestToTSet)
{
    TArray<int32> From({ 1, 2, 3 });

    TSet<int32> Result = Ur::View::Ref(From).To<TSet>();

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestFromTMapToTMap)
{
    TMap<int32, int32> From = { {1,2}, {2,3} };

    TMap<int32, int32> Result = Ur::View::Ref(From).To<TMap>();

    return EqualTo(Result, { TPair<int32, int32>(1, 2), TPair<int32, int32>(2, 3) });
}

UR_TEST(UnrealRanges, To, TestFromMapLikeSequenceToTMap)
{
    TArray<TPair<int32, int32>> From = { TPair<int32, int32>{1,2}, TPair<int32, int32>{2,3} };

    TMap<int32, int32> Result = Ur::View::Ref(From).To<TMap>();

    return EqualTo(Result, { TPair<int32, int32>(1, 2), TPair<int32, int32>(2, 3) });
}

UR_TEST(UnrealRanges, To, TestFromMapLikeSequenceToTMapWithDuplicates)
{
    TArray<TPair<int32, int32>> From = { TPair<int32, int32>{1,2}, TPair<int32, int32>{1,3} };

    TMap<int32, int32> Result = Ur::View::Ref(From).To<TMap>();

    return EqualTo(Result, { TPair<int32, int32>(1, 3) });
}

UR_TEST(UnrealRanges, To, TestFromTMapToTMultiMap)
{
    TMultiMap<int32, int32> From = { {1,2}, {2,3} };

    TMultiMap<int32, int32> Result = Ur::View::Ref(From).To<TMultiMap>();

    return EqualTo(Result, { TPair<int32, int32>(1, 2), TPair<int32, int32>(2, 3) });
}

UR_TEST(UnrealRanges, To, TestFromMapLikeSequenceToTMultiMap)
{
    TArray<TPair<int32, int32>> From = { TPair<int32, int32>{1,2}, TPair<int32, int32>{2,3} };

    TMultiMap<int32, int32> Result = Ur::View::Ref(From).To<TMultiMap>();

    return EqualTo(Result, { TPair<int32, int32>(1, 2), TPair<int32, int32>(2, 3) });
}

UR_TEST(UnrealRanges, To, TestFromMapLikeSequenceToTMultiMapWithDuplicates)
{
    TArray<TPair<int32, int32>> From = { TPair<int32, int32>{1,2}, TPair<int32, int32>{1,3} };

    TMultiMap<int32, int32> Result = Ur::View::Ref(From).To<TMultiMap>();

    return EqualTo(Result, { TPair<int32, int32>{1,2}, TPair<int32, int32>(1, 3) });
}

UR_TEST(UnrealRanges, To, TestToTDeque)
{
    TArray<int32> From({ 1, 2, 3 });

    TDeque<int32> Result = Ur::View::Ref(From).To<TDeque>();

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestToTIndirectArray)
{
    TArray<Ur::TOwned<int32*>> FromPtrs({ Ur::Owned(new int32(1)), Ur::Owned(new int32(2)), Ur::Owned(new int32(3)) });

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result = Ur::View::Ref(FromPtrs).To<TIndirectArray>();

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestToExactlyTIndirectArray)
{
    TArray<Ur::TOwned<int32*>> FromPtrs({ Ur::Owned(new int32(1)), Ur::Owned(new int32(2)), Ur::Owned(new int32(3)) });

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result = Ur::View::Ref(FromPtrs).ToExactly<TIndirectArray<int32>>();

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestIntoTIndirectArray)
{
    TArray<Ur::TOwned<int32*>> FromPtrs({ Ur::Owned(new int32(1)), Ur::Owned(new int32(2)), Ur::Owned(new int32(3)) });

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result;
    Ur::View::Ref(FromPtrs).Into(Result);

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestUniquePtrToTIndirectArray)
{
    TArray<TUniquePtr<int32>> FromPtrs;
    FromPtrs.Add(MakeUnique<int32>(1));
    FromPtrs.Add(MakeUnique<int32>(2));
    FromPtrs.Add(MakeUnique<int32>(3));

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result = Ur::View::Ref(FromPtrs).To<TIndirectArray>();

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestUniquePtrToExactlyTIndirectArray)
{
    TArray<TUniquePtr<int32>> FromPtrs;
    FromPtrs.Add(MakeUnique<int32>(1));
    FromPtrs.Add(MakeUnique<int32>(2));
    FromPtrs.Add(MakeUnique<int32>(3));

    // TIndirectArray takes ownership over passed elements
    TIndirectArray<int32> Result = Ur::View::Ref(FromPtrs).ToExactly<TIndirectArray<int32>>();

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestUniquePtrIntoTIndirectArray)
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

UR_TEST(UnrealRanges, To, TestToTSparseArray)
{
    TArray<int32> From({ 1, 2, 3 });

    TSparseArray<int32> Result = Ur::View::Ref(From).To<TSparseArray>();

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestFromCharSeqToExactlyFString)
{
    TArray<TCHAR> From({ TEXT('A'), TEXT('B'), TEXT('C') });

    FString Result = Ur::View::Ref(From).ToExactly<FString>();

    return EqualTo(Result, FString(TEXT("ABC")));
}

UR_TEST(UnrealRanges, To, TestFromIntSeqToExactlyFString)
{
    TArray<int32> From({ 1, 2, 3 });

    FString Result = Ur::View::Ref(From).ToExactly<FString>();

    return EqualTo(Result, FString(TEXT("123")));
}

UR_TEST(UnrealRanges, To, TestFromFStringSeqToExactlyFString)
{
    TArray<FString> From({ FString(TEXT("123")), FString(TEXT("abc")) });

    FString Result = Ur::View::Ref(From).ToExactly<FString>();

    return EqualTo(Result, FString(TEXT("123abc")));
}




UR_TEST(UnrealRanges, To, TestToExactlyTChunkedArray)
{
    TArray<int32> From({ 1, 2, 3 });

    // TChunkedArray have NTTP parameter thus To<TChunkedArray> isn't possible
    TChunkedArray<int32> Result = Ur::View::Ref(From).ToExactly<TChunkedArray<int32>>();

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestIntoTDoubleLinkedList)
{
    TArray<int32> From({ 1, 2, 3 });

    // TDoubleLinkedList<int32> isn't movable so it should be created beforehand
    TDoubleLinkedList<int32> Result;
    Ur::View::Ref(From).Into(Result);

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestIntoTStaticArray)
{
    TArray<int32> From({ 1, 2, 3 });

    // TStaticArray have NTTP parameter thus To<TStaticArray> isn't possible
    TStaticArray<int32, 3> Result;
    Ur::View::Ref(From).Into(Result);

    return EqualTo(Result, { 1, 2, 3 });
}

UR_TEST(UnrealRanges, To, TestIntoTStaticArrayOverSize)
{
    TArray<int32> From({ 1, 2, 3 });

    // TStaticArray have NTTP parameter thus To<TStaticArray> isn't possible
    TStaticArray<int32, 2> Result;
    Ur::View::Ref(From).Into(Result);

    return EqualTo(Result, { 1, 2 });
}

#endif