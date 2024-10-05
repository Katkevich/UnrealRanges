#if WITH_DEV_AUTOMATION_TESTS

#include "Test.h"

#include "UnrealRanges/View/All.h"
#include "EqualTo.h"
#include "Containers/Array.h"

using namespace Ur::View;

UR_TEST(UnrealRanges, Front, EmptyRangeReturnsEmptyOptionalReference)
{
    TArray<int32> From;

    TOptional<int32&> Result = Ref(From).Front();

    return !Result.IsSet();
}

UR_TEST(UnrealRanges, Front, EmptyRangeReturnsEmptyOptionalValue)
{
    TArray<int32> From;
    TOptional<int32> Result = Transform(From, [](int32& Num) { return Num; }).Front();

    return !Result.IsSet();
}

UR_TEST(UnrealRanges, Front, NonEmptyRangeReturnsNonEmptyOptionalReference)
{
    TArray<int32> From = { 1,2 };

    TOptional<int32&> Result = Ref(From).Front();

    return Result.IsSet() && Result.GetPtrOrNull() == std::addressof(From[0]) && *Result == 1;
}

UR_TEST(UnrealRanges, Front, NonEmptyRangeReturnsNonEmptyOptionalValue)
{
    TArray<int32> From = { 1,2 };
    TOptional<int32> Result = Transform(From, [](int32& Num) { return Num; }).Front();

    return Result.IsSet() && *Result == 1;
}

UR_TEST(UnrealRanges, Back, EmptyRangeReturnsEmptyOptionalReference)
{
    TArray<int32> From;

    TOptional<int32&> Result = Ref(From).Back();

    return !Result.IsSet();
}

UR_TEST(UnrealRanges, Back, EmptyRangeReturnsEmptyOptionalValue)
{
    TArray<int32> From;
    TOptional<int32> Result = Transform(From, [](int32& Num) { return Num; }).Back();

    return !Result.IsSet();
}

UR_TEST(UnrealRanges, Back, NonEmptyRangeReturnsNonEmptyOptionalReference)
{
    TArray<int32> From = { 1,2 };

    TOptional<int32&> Result = Ref(From).Back();

    return Result.IsSet() && Result.GetPtrOrNull() == std::addressof(From[1]) && *Result == 2;
}

UR_TEST(UnrealRanges, Back, NonEmptyRangeReturnsNonEmptyOptionalValue)
{
    TArray<int32> From = { 1,2 };
    TOptional<int32> Result = Transform(From, [](int32& Num) { return Num; }).Back();

    return Result.IsSet() && *Result == 2;
}

#endif