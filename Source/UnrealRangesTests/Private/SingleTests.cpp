// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

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

UR_TEST(UnrealRanges, Single, MakeSingleValue)
{
    // Values
    {
        decltype(auto) Result = Ur::View::Single(FString());
        static_assert(std::is_same_v<decltype(Result), TSingleView<FString>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FString&>);
    }
    {
        FString Str;
        decltype(auto) Result = Ur::View::Single(Str);
        static_assert(std::is_same_v<decltype(Result), TSingleView<FString>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FString&>);
    }
    {
        const FString Str;
        decltype(auto) Result = Ur::View::Single(Str);
        static_assert(std::is_same_v<decltype(Result), TSingleView<FString>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FString&>);
    }
    {
        FMoveOnly Str;
        decltype(auto) Result = Ur::View::Single(MoveTemp(Str));
        static_assert(std::is_same_v<decltype(Result), TSingleView<FMoveOnly>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FMoveOnly&>);
    }

    return true;
}

UR_TEST(UnrealRanges, Single, MixWithConcat)
{
    TArray<FString> Array = { TEXT("1"), TEXT("abba1") , TEXT("abba2"), TEXT("2") };

    auto Result = Ref(Array)
        .Filter([](const FString& Str) { return Str.Len() > 1; })
        .ConcatWith(Single(FString(TEXT("single1"))), Single(FString(TEXT("single2"))))
        .Transform([](FString& Str) { return Str.RightChop(1); });

    return EqualTo(Result, { TEXT("bba1") , TEXT("bba2"), TEXT("ingle1"), TEXT("ingle2") });
}

UR_TEST(UnrealRanges, Single, MakeSingleRef)
{
    FMoveOnly Obj;
    const FMoveOnly ConstObj;
    {
        decltype(auto) Result = Ur::View::SingleRef(Obj);
        static_assert(std::is_same_v<decltype(Result), TSingleView<FMoveOnly&>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FMoveOnly&>);
    }
    {
        decltype(auto) Result = Ur::View::SingleRef(ConstObj);
        static_assert(std::is_same_v<decltype(Result), TSingleView<const FMoveOnly&>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), const FMoveOnly&>);
    }
    {
        // won't compile
        //decltype(auto) Result = Ur::View::SingleRef(FMoveOnly{});
    }
    {
        std::reference_wrapper<const FMoveOnly> Ref(ConstObj);
        decltype(auto) Result = Ur::View::Single(Ref);
        static_assert(std::is_same_v<decltype(Result), TSingleView<const FMoveOnly&>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), const FMoveOnly&>);
    }
    {
        std::reference_wrapper<FMoveOnly> Ref(Obj);
        decltype(auto) Result = Ur::View::Single(Ref);
        static_assert(std::is_same_v<decltype(Result), TSingleView<FMoveOnly&>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FMoveOnly&>);
    }
    {
        decltype(auto) Result = Ur::View::Single(std::reference_wrapper<FMoveOnly>{ Obj });
        static_assert(std::is_same_v<decltype(Result), TSingleView<FMoveOnly&>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), FMoveOnly&>);
    }
    {
        decltype(auto) Result = Ur::View::Single(std::cref(Obj));
        static_assert(std::is_same_v<decltype(Result), TSingleView<const FMoveOnly&>>);
        static_assert(std::is_same_v<decltype(*Result.begin()), const FMoveOnly&>);
    }

    return true;
}

UR_TEST(UnrealRanges, Single, MixSingleRefWithConcat)
{
    TArray<FString> Array = { TEXT("1"), TEXT("abba1") , TEXT("abba2"), TEXT("2") };
    FString Single1 = TEXT("single1");
    FString Single2 = TEXT("single2");

    auto Result = Ref(Array)
        .Filter([](const FString& Str) { return Str.Len() > 1; })
        .ConcatWith(SingleRef(Single1), Single(std::cref(Single2)))
        .Transform([](const FString& Str) { return Str.RightChop(1); });

    static_assert(std::is_same_v<decltype(*Result.begin()), FString>);

    return EqualTo(Result, { TEXT("bba1") , TEXT("bba2"), TEXT("ingle1"), TEXT("ingle2") });
}

UR_TEST(UnrealRanges, Single, SingleViewInternalIteration)
{
    auto Result = Single(FString(TEXT("abba")))
        .To<TArray>();

    return EqualTo(Result, { FString(TEXT("abba")) });
}

#endif