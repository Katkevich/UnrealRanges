#pragma once
#include <initializer_list>

template<typename TRng, typename TExpectedValue>
bool EqualTo(TRng&& Actual, std::initializer_list<TExpectedValue> Expected)
{
    bool bAllEqual = true;
    auto ExpectedIt = Expected.begin();

    for (auto&& Item : Actual)
    {
        bAllEqual &= Item == *ExpectedIt++;
    }

    return bAllEqual;
}

template<typename TRng>
bool EqualTo(TRng&& Actual, FString Expected)
{
    bool bAllEqual = true;
    auto ExpectedIt = Expected.begin();

    for (auto&& Item : Actual)
    {
        bAllEqual &= Item == *ExpectedIt++;
    }

    return bAllEqual;
}