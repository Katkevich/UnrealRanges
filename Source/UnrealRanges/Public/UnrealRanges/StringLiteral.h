#pragma once
#include "Containers/UnrealString.h"

FString operator""_s(const TCHAR * Str, std::size_t Len)
{
    return FString(Len, Str);
}

FStringView operator""_sv(const TCHAR * Str, std::size_t)
{
    return FStringView(Str);
}