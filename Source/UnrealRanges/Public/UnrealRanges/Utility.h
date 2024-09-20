#pragma once
#include "UnrealRanges/Detail/AlwaysFalse.h"
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "Templates/UnrealTemplate.h"
#include <concepts>

namespace Ur {
    template<typename TRng>
    concept HasBegin = requires(TRng Rng) {
        requires requires { Rng.begin(); } || requires { begin(Rng); };
    };

    template<typename TRng>
    concept HasEnd = requires(TRng Rng) {
        requires requires { Rng.end(); } || requires { end(Rng); };
    };

    template<typename TRng>
    concept HasRBegin = requires(TRng Rng) {
        requires requires { Rng.rbegin(); } || requires { rbegin(Rng); };
    };

    template<typename TRng>
    concept HasREnd = requires(TRng Rng) {
        requires requires { Rng.rend(); } || requires { rend(Rng); };
    };

    template<typename TRng>
    concept HasSize = requires(TRng Rng) {
        requires requires { Rng.Num(); } || requires { Rng.Len(); };
    };


    struct FBeginCpo
    {
        template<HasBegin TRng>
        auto operator()(TRng&& Rng) const
        {
            using std::begin;
            return begin(UR_FWD(Rng));
        }
    };

    struct FRBeginCpo
    {
        template<HasRBegin TRng>
        auto operator()(TRng&& Rng) const
        {
            using std::rbegin;
            return rbegin(UR_FWD(Rng));
        }
    };

    struct FEndCpo
    {
        template<HasEnd TRng>
        auto operator()(TRng&& Rng) const
        {
            using std::end;
            return end(UR_FWD(Rng));
        }
    };

    struct FREndCpo
    {
        template<HasREnd TRng>
        auto operator()(TRng&& Rng) const
        {
            using std::rend;
            return rend(UR_FWD(Rng));
        }
    };

    struct FSizeCpo
    {
        template<HasSize TRng>
        auto operator()(TRng&& Rng) const
        {
            if constexpr (requires { Rng.Num(); })
                return Rng.Num();
            else if constexpr (requires { Rng.Len(); })
                return Rng.Len();
            else
                return GetNum(UR_FWD(Rng));
        }
    };

    struct FDataCpo
    {
        template<typename TRng>
        auto operator()(TRng&& Rng) const
        {
            return GetData(UR_FWD(Rng));
        }
    };

    // internal iteration
    namespace Misc {

        static constexpr bool Forward = true;
        static constexpr bool Reverse = false;

        template<bool Direction>
        static constexpr bool Opposite = !Direction;
    }

    template<bool IsForward>
    static constexpr std::conditional_t<IsForward, FBeginCpo, FRBeginCpo> BeginEx{};

    template<bool IsForward>
    static constexpr std::conditional_t<IsForward, FEndCpo, FREndCpo> EndEx{};

    // use custom Begin & End coz UE isn't very consistent defining them (see TDoubleLinkedList which has begin & end as private friends)
    static constexpr FBeginCpo Begin{};
    static constexpr FEndCpo End{};

    static constexpr FRBeginCpo RBegin{};
    static constexpr FREndCpo REnd{};

    static constexpr FSizeCpo Size{};
    static constexpr FDataCpo Data{};


    template <typename TLike, typename T>
    constexpr decltype(auto) AsConstLike(T& Value) noexcept
    {
        if constexpr (std::is_const_v<TLike>)
            return std::as_const(Value);
        else
            return (Value);
    }
}