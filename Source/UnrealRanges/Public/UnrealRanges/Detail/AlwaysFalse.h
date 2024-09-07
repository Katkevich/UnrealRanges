#pragma once

namespace Ur::Detail {
template<typename T>
struct TAlwaysFalse
{
    static constexpr bool Value = false;
};
}