#pragma once
#include "UnrealRanges/Detail/AlwaysFalse.h"
#include "UnrealRanges/Detail/ForwardMacro.h"
#include <concepts>

namespace Ur {
    template<typename TRng>
    concept HasBegin = requires(TRng Rng) {
        [](TRng& Rng) { using std::begin; begin(Rng); }(Rng);
    };

    template<typename TRng>
    concept HasEnd = requires(TRng Rng) {
        [](TRng& Rng) { using std::end; end(Rng); }(Rng);
    };

    template<typename TRng>
    concept HasRBegin = requires(TRng Rng) {
        [](TRng& Rng) { using std::rbegin; rbegin(Rng); }(Rng);
    };

    template<typename TRng>
    concept HasREnd = requires(TRng Rng) {
        [](TRng& Rng) { using std::rend; rend(Rng); }(Rng);
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
        template<typename TRng>
        auto operator()(TRng&& Rng) const
        {
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
        template<bool IsForward>
        struct FDirection
        {
        };

        static constexpr auto Forward = FDirection<true>{};
        static constexpr auto Reverse = FDirection<false>{};

        template<bool IsForward>
        static constexpr auto Opposite = FDirection<!IsForward>{};

        template<bool IsForward>
        static constexpr auto Same = FDirection<IsForward>{};
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
}