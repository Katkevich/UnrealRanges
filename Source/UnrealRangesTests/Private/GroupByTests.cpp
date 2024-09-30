// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

template<typename T>
struct Foo;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_GroupByHasCorrectReferenceTypes, "UnrealRanges.GroupBy.TestGroupByHasCorrectReferenceTypes", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_GroupByHasCorrectReferenceTypes::RunTest(const FString& Parameters)
{
    auto Pred = [](const FString& Str) { return Str.Len(); };

    TArray<FString> From;

    auto Result = GroupBy(From, Pred);
    const auto ConstResult = GroupBy(From, Pred);

    using GroupByViewType = TGroupByView<TRefView<TArray<FString>>, decltype(Pred)>;

    static_assert(std::is_same_v<decltype(Result)::reference, TGroupView<GroupByViewType, GroupByViewType::Cursor>>);
    static_assert(std::is_same_v<decltype(Result)::const_reference, TGroupView<GroupByViewType, GroupByViewType::ConstCursor>>);
    static_assert(std::is_same_v<decltype(*Result.begin()), TGroupView<GroupByViewType, GroupByViewType::Cursor>>);
    static_assert(std::is_same_v<decltype(*Result.cbegin()), TGroupView<GroupByViewType, GroupByViewType::ConstCursor>>);

    static_assert(std::is_same_v<decltype(ConstResult)::reference, TGroupView<GroupByViewType, GroupByViewType::Cursor>>);
    static_assert(std::is_same_v<decltype(ConstResult)::const_reference, TGroupView<GroupByViewType, GroupByViewType::ConstCursor>>);
    static_assert(std::is_same_v<decltype(*ConstResult.begin()), TGroupView<GroupByViewType, GroupByViewType::ConstCursor>>);
    static_assert(std::is_same_v<decltype(*ConstResult.cbegin()), TGroupView<GroupByViewType, GroupByViewType::ConstCursor>>);

    // non-const array -> non-const view -> non-const group -> non-const item
    for (auto Group : Result)
    {
        static_assert(std::is_same_v<decltype(Group), TGroupView<GroupByViewType, GroupByViewType::Cursor>>);

        static_assert(std::is_same_v<decltype(Group)::reference, FString&>);
        static_assert(std::is_same_v<decltype(Group)::const_reference, const FString&>);
        static_assert(std::is_same_v<decltype(*Group.begin()), FString&>);
        static_assert(std::is_same_v<decltype(*Group.cbegin()), const FString&>);

        for (FString& Str : Group)
        {
        }
    }

    // non-const array -> non-const view -> non-const group -> const item
    for (auto Group : Result)
    {
        static_assert(std::is_same_v<decltype(Group), TGroupView<GroupByViewType, GroupByViewType::Cursor>>);

        static_assert(std::is_same_v<decltype(Group)::reference, FString&>);
        static_assert(std::is_same_v<decltype(Group)::const_reference, const FString&>);
        static_assert(std::is_same_v<decltype(*Group.begin()), FString&>);
        static_assert(std::is_same_v<decltype(*Group.cbegin()), const FString&>);

        for (const FString& Str : Group)
        {
        }
    }

    // non-const array -> non-const view -> const group -> const item
    for (const auto Group : Result)
    {
        static_assert(std::is_same_v<decltype(Group), const TGroupView<GroupByViewType, GroupByViewType::Cursor>>);

        static_assert(std::is_same_v<decltype(Group)::reference, FString&>);
        static_assert(std::is_same_v<decltype(Group)::const_reference, const FString&>);
        static_assert(std::is_same_v<decltype(*Group.begin()), const FString&>);
        static_assert(std::is_same_v<decltype(*Group.cbegin()), const FString&>);

        for (const FString& Str : Group)
        {
        }
    }

    // non-const array -> const view -> const group -> const item
    for (const auto Group : ConstResult)
    {
        static_assert(std::is_same_v<decltype(Group), const TGroupView<GroupByViewType, GroupByViewType::ConstCursor>>);

        static_assert(std::is_same_v<decltype(Group)::reference, const FString&>);
        static_assert(std::is_same_v<decltype(Group)::const_reference, const FString&>);
        static_assert(std::is_same_v<decltype(*Group.begin()), const FString&>);
        static_assert(std::is_same_v<decltype(*Group.cbegin()), const FString&>);

        for (const FString& Str : Group)
        {
        }
    }

    // even tho the GroupView itself isn't const because of the fact that it was created from the const GroupByView makes elements of this GroupView also const (note ConstCursor)
    // non-const array -> const view -> const group -> const item
    for (auto Group : ConstResult)
    {
        static_assert(std::is_same_v<decltype(Group), TGroupView<GroupByViewType, GroupByViewType::ConstCursor>>);

        static_assert(std::is_same_v<decltype(Group)::reference, const FString&>);
        static_assert(std::is_same_v<decltype(Group)::const_reference, const FString&>);
        static_assert(std::is_same_v<decltype(*Group.begin()), const FString&>);
        static_assert(std::is_same_v<decltype(*Group.cbegin()), const FString&>);

        for (const FString& Str : Group)
        {
        }
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_GroupByProducesLValueReference, "UnrealRanges.GroupBy.TestGroupByProducesLValueReference", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_GroupByProducesLValueReference::RunTest(const FString& Parameters)
{
    bool bResult = true;

    TArray<FString> From = { TEXT("1"), TEXT("2"), TEXT("33"), TEXT("44"), TEXT("5"), TEXT("55"), TEXT("66"), TEXT("77") };
    TArray<TArray<FString>> Expected = {
        { TEXT("1"), TEXT("2") },
        { TEXT("33"), TEXT("44") },
        { TEXT("5") },
        { TEXT("55"), TEXT("66"), TEXT("77") }
    };

    auto Result = GroupBy(From, &FString::Len);

    auto ExpectedGroupIt = Expected.begin();

    for (auto Group : Result)
    {
        auto ExpectedItemIt = (*ExpectedGroupIt).begin();

        for (FString& Str : Group)
        {
            bResult &= (Str == *ExpectedItemIt);
            ++ExpectedItemIt;
        }
        ++ExpectedGroupIt;
    }

    return bResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_GroupByInternalIteration, "UnrealRanges.GroupBy.TestGroupByInternalIteration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_GroupByInternalIteration::RunTest(const FString& Parameters)
{
    bool bResult = true;

    TArray<FString> From = { TEXT("1"), TEXT("2"), TEXT("33"), TEXT("44"), TEXT("5"), TEXT("55"), TEXT("66"), TEXT("77") };
    TArray<TArray<FString>> Expected = {
        { TEXT("1"), TEXT("2") },
        { TEXT("33"), TEXT("44") },
        { TEXT("5") },
        { TEXT("55"), TEXT("66"), TEXT("77") }
    };

    auto Result = GroupBy(From, &FString::Len)
        .To<TArray>();

    auto ExpectedGroupIt = Expected.begin();

    for (auto Group : Result)
    {
        auto ExpectedItemIt = (*ExpectedGroupIt).begin();

        for (FString& Str : Group)
        {
            bResult &= (Str == *ExpectedItemIt);
            ++ExpectedItemIt;
        }
        ++ExpectedGroupIt;
    }

    return bResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_GroupViewFromGroupByInternalIteration, "UnrealRanges.GroupBy.TestGroupViewFromGroupByInternalIteration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_GroupViewFromGroupByInternalIteration::RunTest(const FString& Parameters)
{
    bool bResult = true;

    TArray<FString> From = { TEXT("1"), TEXT("2"), TEXT("33"), TEXT("44"), TEXT("5"), TEXT("55"), TEXT("66"), TEXT("77") };
    TArray<TArray<FString>> Expected = {
        { TEXT("1"), TEXT("2") },
        { TEXT("33"), TEXT("44") },
        { TEXT("5") },
        { TEXT("55"), TEXT("66"), TEXT("77") }
    };

    auto Result = Ref(From)
        .GroupBy(&FString::Len)
        .Transform([](auto Group) { return Group.To<TArray>(); });

    auto ExpectedGroupIt = Expected.begin();

    for (auto Group : Result)
    {
        auto ExpectedItemIt = (*ExpectedGroupIt).begin();

        for (FString& Str : Group)
        {
            bResult &= (Str == *ExpectedItemIt);
            ++ExpectedItemIt;
        }
        ++ExpectedGroupIt;
    }

    return bResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_GroupByOverRValues, "UnrealRanges.GroupBy.TestGroupByOverRValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_GroupByOverRValues::RunTest(const FString& Parameters)
{
    bool bResult = true;

    const TCHAR* Chars = TEXT("abcdef");
    TArray<int32> From = { 1,1,2,2,1,3,3,3 };

    TArray<TArray<FString>> Expected = {
        { TEXT("a"), TEXT("a") },
        { TEXT("ab"), TEXT("ab") },
        { TEXT("a") },
        { TEXT("abc"), TEXT("abc"), TEXT("abc") }
    };

    auto Result = Ref(From)
        .Transform([&](int32 Num) { return FString(Num, Chars); })
        .GroupBy(&FString::Len);

    using GroupByViewType = decltype(Result);

    auto ExpectedGroupIt = Expected.begin();

    for (auto Group : Result)
    {
        static_assert(std::is_same_v<decltype(Group), TGroupView<GroupByViewType, GroupByViewType::Cursor>>);
        static_assert(std::is_same_v<decltype(Group.GroupKey()), int32>);

        static_assert(std::is_same_v<decltype(Group)::reference, FString>);
        static_assert(std::is_same_v<decltype(Group)::const_reference, FString>);
        static_assert(std::is_same_v<decltype(*Group.begin()), FString>);
        static_assert(std::is_same_v<decltype(*Group.cbegin()), FString>);

        auto ExpectedItemIt = (*ExpectedGroupIt).begin();

        for (const FString& Str : Group) // FString& won't and shouldn't compile
        {
            bResult &= (Str == *ExpectedItemIt);
            ++ExpectedItemIt;
        }
        ++ExpectedGroupIt;
    }

    return bResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnrealRangesTests_GroupByFromFilterAndReverseView, "UnrealRanges.GroupBy.TestGroupByFromFilterAndReverseView", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnrealRangesTests_GroupByFromFilterAndReverseView::RunTest(const FString& Parameters)
{
    bool bResult = true;

    TArray<FString> From = { TEXT("11"), TEXT("12"), TEXT("44"), TEXT("133"), TEXT("5"), TEXT("155"), TEXT("166"), TEXT("222"), TEXT("177")};
    TArray<TArray<FString>> Expected = {
        { TEXT("177"), TEXT("166"), TEXT("155"), TEXT("133") },
        { TEXT("12"), TEXT("11") }
    };

    auto Result = Ref(From)
        .Filter([](const FString& Str) { return Str[0] == '1'; })
        .Reverse()
        .GroupBy(&FString::Len);

    using GroupByViewType = decltype(Result);

    auto ExpectedGroupIt = Expected.begin();

    for (auto Group : Result)
    {
        static_assert(std::is_same_v<decltype(Group), TGroupView<GroupByViewType, GroupByViewType::Cursor>>);
        static_assert(std::is_same_v<decltype(Group.GroupKey()), int32>);

        static_assert(std::is_same_v<decltype(Group)::reference, FString&>);
        static_assert(std::is_same_v<decltype(Group)::const_reference, const FString&>);
        static_assert(std::is_same_v<decltype(*Group.begin()), FString&>);
        static_assert(std::is_same_v<decltype(*Group.cbegin()), const FString&>);

        auto ExpectedItemIt = (*ExpectedGroupIt).begin();

        for (const FString& Str : Group) // FString& won't and shouldn't compile
        {
            bResult &= (Str == *ExpectedItemIt);
            ++ExpectedItemIt;
        }
        ++ExpectedGroupIt;
    }

    return bResult;
}

#endif