#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include "UnrealRanges/Optional.h"

namespace Ur::View {
    template<typename TValue>
    class TMaybeView
        : public FView
        , public Detail::TMixins<TMaybeView<TValue>, TDefaultMixins>
        , public Detail::TMixins<TMaybeView<TValue>, TBidirMixins>
        , public Detail::TMixins<TMaybeView<TValue>, TSizedMixins>
        , public Detail::TConditionalMixins<TIsPair_V<std::remove_cvref_t<TValue>>, TMaybeView<TValue>, TMapMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = std::add_lvalue_reference_t<TValue>;
        using const_reference = std::add_lvalue_reference_t<const TValue>;
        using value_type = std::remove_cvref_t<TValue>;
        using pointer = std::add_pointer_t<reference>;
        using const_pointer = std::add_pointer_t<const_reference>;

        using Cursor = pointer;
        using ConstCursor = const_pointer;
        using ReverseCursor = pointer;
        using ReverseConstCursor = const_pointer;

        static constexpr bool IsBidir = true;
        static constexpr bool IsSized = true;
        static constexpr bool LikeMap = TIsPair_V<value_type>;

        template<typename UValue>
        TMaybeView(Misc::FFromViewTag, const TOptional<UValue>& InValue)
            : Value(InValue)
        {
        }

        template<typename UValue>
        TMaybeView(Misc::FFromViewTag, TOptional<UValue>&& InValue)
            : Value(MoveTemp(InValue))
        {
        }

        TMaybeView(Misc::FFromViewTag, FNullOpt)
        {
        }


        auto Num() const
        {
            return static_cast<int32>(Value.IsSet());
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            if (Self.Value)
                return Callback(*Self.Value);
            else
                return Ur::ELoop::Continue;
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return Self.Value.GetPtrOrNull();
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf&)
        {
            return static_cast<View::TCursor<TSelf, IsForward>>(nullptr);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            Curs = nullptr;
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return *Curs;
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return Lhs == Rhs;
        }

    private:
        TOptional<TValue> Value;
    };


    // store value (copy/move)
    template<typename T>
    auto MaybeVal(T&& Value)
    {
        using TValue = std::remove_cvref_t<T>;
        return TMaybeView<TValue>({}, TOptional<TValue>(UR_FWD(Value)));
    }

    // store value (copy/move) in True otherwise empty
    template<typename T>
    auto MaybeVal(T&& Value, bool bCondition)
    {
        using TValue = std::decay_t<T>;
        return TMaybeView<TValue>({}, bCondition ? TOptional<TValue>(UR_FWD(Value)) : TOptional<TValue>(NullOpt));
    }

    // store reference to value
    template<typename T>
    auto MaybeRef(T& Value)
    {
        return TMaybeView<T&>({}, TOptional<T&>(Value));
    }

    // store reference to optional's inner-value
    template<typename T>
    auto MaybeRef(const TOptional<T>& Value)
    {
        using TRef = std::add_lvalue_reference_t<T>;
        return TMaybeView<TRef>({}, Value ? TOptional<TRef>(Value.GetValue()) : TOptional<TRef>(NullOpt));
    }

    // store reference to optional's inner-value
    template<typename T>
    auto MaybeRef(TOptional<T>& Value)
    {
        using TRef = std::add_lvalue_reference_t<T>;
        return TMaybeView<TRef>({}, Value ? TOptional<TRef>(Value.GetValue()) : TOptional<TRef>(NullOpt));
    }

    // store reference to value if True
    template<typename T>
    auto MaybeRef(T& Value, bool bCondition)
    {
        return TMaybeView<T&>({}, bCondition ? TOptional<T&>(Value) : TOptional<T&>(NullOpt));
    }

    // store reference to optional's inner-value if True otherwise empty
    template<typename T>
    auto MaybeRef(const TOptional<T>& Value, bool bCondition)
    {
        using TValue = std::add_lvalue_reference_t<T>;
        return TMaybeView<TValue>({}, Value && bCondition ? TOptional<TValue>(Value.GetValue()) : TOptional<TValue>(NullOpt));
    }

    // store reference to optional's inner-value if True otherwise empty
    template<typename T>
    auto MaybeRef(TOptional<T>& Value, bool bCondition)
    {
        using TValue = std::add_lvalue_reference_t<T>;
        return TMaybeView<TValue>({}, Value && bCondition ? TOptional<TValue>(Value.GetValue()) : TOptional<TValue>(NullOpt));
    }

    // store optional as-is (reference or value)
    template<typename TOptionalIn>
    auto Maybe(TOptionalIn&& InValue) requires THasTemplate_V<TOptionalIn, TOptional> // has to be optional
    {
        using TOpt = std::remove_cvref_t<TOptionalIn>;
        using TValue = typename TOpt::ElementType;

        static_assert(!std::is_rvalue_reference_v<TValue>, "cannot be rvalue reference");

        return TMaybeView<TValue>({}, UR_FWD(InValue));
    }

    // store optional as-is (reference or value) if True otherwise empty
    template<typename TOptionalIn>
    auto Maybe(TOptionalIn&& InValue, bool bCondition) requires THasTemplate_V<TOptionalIn, TOptional> // has to be optional
    {
        using TOpt = std::remove_cvref_t<TOptionalIn>;
        using TValue = typename TOpt::ElementType;

        static_assert(!std::is_rvalue_reference_v<TValue>, "cannot be rvalue reference");

        return TMaybeView<TValue>({}, bCondition ? TOpt(UR_FWD(InValue)) : TOpt(NullOpt));
    }

    // store reference to value if not nullptr otherwise empty
    template<typename T>
    auto Maybe(T* InValue)
    {
        return TMaybeView<T&>({}, InValue ? TOptional<T&>(*InValue) : TOptional<T&>(NullOpt));
    }

}