#pragma once

namespace Ur::Detail {
template<typename T>
struct FAlwaysFalse
{
    static constexpr bool Value = false;
};
}