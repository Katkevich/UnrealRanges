#pragma once
#include "UnrealRanges/View/Mixin/Transform.h"
#include "UnrealRanges/View/Mixin/Filter.h"
#include "UnrealRanges/View/Mixin/Enumerate.h"
#include "UnrealRanges/View/Mixin/Take.h"
#include "UnrealRanges/View/Mixin/TakeWhile.h"
#include "UnrealRanges/View/Mixin/Concat.h"
#include "UnrealRanges/View/Mixin/Reverse.h"
#include "UnrealRanges/View/Mixin/Iterator.h"
#include "UnrealRanges/View/AlgoMixin/To.h"
#include "UnrealRanges/View/AlgoMixin/MinMax.h"
#include "UnrealRanges/View/AlgoMixin/Find.h"
#include "UnrealRanges/View/AlgoMixin/Count.h"
#include "UnrealRanges/View/AlgoMixin/Fold.h"
#include "UnrealRanges/View/AlgoMixin/Sum.h"
#include "UnrealRanges/View/View.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "UnrealRanges/Detail/EmptyBase.h"
#include "UnrealRanges/Detail/CursorProtocol.h"
#include <memory>

#ifndef UR_DEBUG_NOINLINE
#define UR_DEBUG_NOINLINE //FORCENOINLINE
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
        , public TTransformMixin<TRefView<TRng>>
        , public TFilterMixin<TRefView<TRng>>
        , public TEnumerateMixin<TRefView<TRng>>
        , public TTakeMixin<TRefView<TRng>>
        , public TTakeWhileMixin<TRefView<TRng>>
        , public TConcatMixin<TRefView<TRng>>
        , public TToMixin<TRefView<TRng>>
        , public TMinMaxMixin<TRefView<TRng>>
        , public TFindFirstMixin<TRefView<TRng>>
        , public TConditionalInheritance<BiDirRange<TRng>, TFindLastMixin<TRefView<TRng>>>
        , public TCountMixin<TRefView<TRng>>
        , public TFoldLeftMixin<TRefView<TRng>>
        , public TConditionalInheritance<BiDirRange<TRng>, TFoldRightMixin<TRefView<TRng>>>
        , public TSumMixin<TRefView<TRng>>
        , public TIteratorMixin<TRefView<TRng>>
        , public TConditionalInheritance<BiDirRange<TRng>, TReverseMixin<TRefView<TRng>>>
        , public TConditionalInheritance<BiDirRange<TRng>, TReverseIteratorMixin<TRefView<TRng>>>
    {
        friend struct FCursorProtocol;

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
        UR_DEBUG_NOINLINE static Misc::ELoop InternalIteration(TSelf& Self, TFn Fn)
        {
            for (auto It = CursorBegin<IsForward>(Self); It != CursorEnd<IsForward>(Self); ++It)
                if (Fn(*It) == Misc::ELoop::Break)
                    return Misc::ELoop::Break;

            return Misc::ELoop::Continue;
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
