#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include "UnrealRanges/Optional.h"

namespace Ur::View {
    template<typename TValue>
    class TSingleView
        : public FView
        , public Detail::TMixins<TSingleView<TValue>, TDefaultMixins>
        , public Detail::TMixins<TSingleView<TValue>, TBidirMixins>
        , public Detail::TMixins<TSingleView<TValue>, TSizedMixins>
        , public Detail::TConditionalMixins<TIsPair_V<std::remove_cvref_t<TValue>>, TSingleView<TValue>, TMapMixins>
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
        TSingleView(Misc::FFromViewTag, UValue&& InValue)
            : Value(UR_FWD(InValue))
        {
        }


        auto Num() const
        {
            return 1;
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            return Callback(Self.Value);
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return std::addressof(Self.Value);
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
        TValue Value;
    };


    template<typename TValue>
    class TSingleView<TValue&>
        : public FView
        , public Detail::TMixins<TSingleView<TValue&>, TDefaultMixins>
        , public Detail::TMixins<TSingleView<TValue&>, TBidirMixins>
        , public Detail::TMixins<TSingleView<TValue&>, TSizedMixins>
        , public Detail::TConditionalMixins<TIsPair_V<std::remove_cvref_t<TValue>>, TSingleView<TValue&>, TMapMixins>
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
        TSingleView(Misc::FFromViewTag, UValue& InValue)
            : Value(std::addressof(InValue))
        {
        }


        auto Num() const
        {
            return 1;
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            return Callback(*Self.Value);
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return Self.Value;
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
        TValue* Value;
    };



    template<typename T>
    auto Single(T&& Value)
    {
        if constexpr (!THasTemplate_V<T, std::reference_wrapper>)
        {
            using TValue = std::decay_t<T>;
            return TSingleView<TValue>({}, UR_FWD(Value));
        }
        else
        {
            using TValue = std::add_lvalue_reference_t<typename std::remove_cvref_t<T>::type>;
            return TSingleView<TValue&>({}, Value.get());
        }
    }

    template<typename T>
    auto SingleRef(T& Value)
    {
        return TSingleView<T&>({}, Value);
    }
}