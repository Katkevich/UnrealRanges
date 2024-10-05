#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include "UnrealRanges/Optional.h"
#include <concepts>

namespace Ur::View {
    template<std::integral TValue>
    class TIotaView
        : public FView
        , public Detail::TMixins<TIotaView<TValue>, TDefaultMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = TValue;
        using const_reference = TValue;
        using value_type = TValue;

        struct Cursor
        {
            TOptional<TValue> Value{};
        };
        struct ConstCursor
        {
            TOptional<TValue> Value{};
        };
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = false;
        static constexpr bool LikeMap = false;

        template<std::integral UValue>
        TIotaView(UValue InFrom, UValue InStep)
            : From(InFrom)
            , Step(InStep)
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TFn>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TFn Fn)
        {
            for (TValue It = Self.From; ; It += Self.Step)
                if (Fn(It) == Ur::ELoop::Break)
                    return Ur::ELoop::Break;

            return Ur::ELoop::Continue;
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return TCursor<TSelf, IsForward>{ TOptional<TValue>(Self.From) };
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return TCursor<TSelf, IsForward>{ NullOpt };
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            (*Curs.Value) += Self.Step;
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static auto CursorDeref(TSelf& Self, const TCursor& Curs) // explicit copy here
        {
            return *Curs.Value;
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return Lhs.Value.IsSet() == Rhs.Value.IsSet() && *Lhs.Value == *Rhs.Value;
        }

    private:
        TValue From;
        TValue Step;
    };


    template<std::integral T>
    auto Iota(T From, T Step = 1)
    {
        return TIotaView<T>(From, Step);
    }

    template<std::integral T>
    auto Ints(T From, T Step = 1)
    {
        return TIotaView<T>(From, Step);
    }
}