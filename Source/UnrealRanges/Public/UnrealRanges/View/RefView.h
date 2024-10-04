#pragma once
#include "UnrealRanges/View/Mixin/Mixin.h"
#include "UnrealRanges/View/View.h"
#include "UnrealRanges/View/Cursor.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include "UnrealRanges/Detail/ForwardMacro.h"
#include <memory>

#ifndef UR_DEBUG_NOINLINE
#define UR_DEBUG_NOINLINE FORCENOINLINE
#endif

namespace Ur::View {
    namespace Detail {
        template<bool IsBidir, bool IsConst, typename TRng>
        struct TReverseCursor
        {
            using Type = void;
        };

        template<typename TRng>
        struct TReverseCursor<true, false, TRng>
        {
            using Type = RangeReverseIterator<TRng>;
        };

        template<typename TRng>
        struct TReverseCursor<true, true, TRng>
        {
            using Type = RangeReverseConstIterator<TRng>;
        };

        template<bool IsBidir, bool IsConst, typename TRng>
        using TReverseCursorType = typename TReverseCursor<IsBidir, IsConst, TRng>::Type;
    }

    template<typename TRng>
    class TRefView
        : public FView
        , public Detail::TMixins<TRefView<TRng>, TDefaultMixins>
        , public Detail::TConditionalMixins<BiDirRange<TRng>, TRefView<TRng>, TBidirMixins>
        , public Detail::TConditionalMixins<TIsPair_V<RangeValue<TRng>>, TRefView<TRng>, TMapMixins>
    {
        friend struct Ur::Cursor;

    public:
        using value_type = RangeValue<TRng>;
        using reference = RangeReference<TRng>;
        using const_reference = RangeConstReference<TRng>;

        using Cursor = RangeIterator<TRng>;
        using ConstCursor = RangeConstIterator<TRng>;
        using ReverseCursor = Detail::TReverseCursorType<BiDirRange<TRng>, false, TRng>;
        using ReverseConstCursor = Detail::TReverseCursorType<BiDirRange<TRng>, true, TRng>;

        static constexpr bool IsBidir = BiDirRange<TRng>;
        static constexpr bool IsSized = SizedRange<TRng>;
        static constexpr bool LikeMap = TIsPair_V<value_type>;

        template<typename URng>
        TRefView(Misc::FFromViewTag, URng& InRng)
            : Rng(std::addressof(InRng))
        {
        }

        auto Num() const requires IsSized
        {
            return Ur::Size(*Rng);
        }

    private:
        template<bool IsForward, typename TSelf, typename TFn>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TFn Fn)
        {
            for (auto It = CursorBegin<IsForward>(Self); It != CursorEnd<IsForward>(Self); ++It)
                if (Fn(*It) == Ur::ELoop::Break)
                    return Ur::ELoop::Break;

            return Ur::ELoop::Continue;
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return Ur::BeginEx<IsForward>(AsConstLike<TSelf>(*Self.Rng));
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return Ur::EndEx<IsForward>(AsConstLike<TSelf>(*Self.Rng));
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            ++Curs;
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return *Curs;
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return !(Lhs != Rhs);
        }

    private:
        TRng* Rng{};
    };


    template<typename TRng>
    auto Ref(TRng& Rng)
    {
        return TRefView<TRng>({}, Rng);
    }

    template<typename TRng>
    auto Cref(const TRng& Rng)
    {
        return TRefView<const TRng>({}, Rng);
    }
}
