#pragma once
#include "UnrealRanges/View/Mixin/Transform.h"
#include "UnrealRanges/View/Mixin/Filter.h"
#include "UnrealRanges/View/Mixin/Reverse.h"
#include "UnrealRanges/View/Mixin/Iterator.h"
#include "UnrealRanges/View/AlgoMixin/To.h"
#include "UnrealRanges/View/AlgoMixin/MinMax.h"
#include "UnrealRanges/View/AlgoMixin/Find.h"
#include "UnrealRanges/View/AlgoMixin/FindLast.h"
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
        template<bool IsBidir, typename TRng>
        struct TReverseCursor
        {
            using Type = void;
        };

        template<typename TRng>
        struct TReverseCursor<true, TRng>
        {
            using Type = RangeReverseIterator<TRng>;
        };

        template<bool IsBidir, typename TRng>
        using TReverseCursorType = typename TReverseCursor<IsBidir, TRng>::Type;
    }

    template<typename TRng>
    class TRefView
        : public FView
        , public TTransformMixin<TRefView<TRng>>
        , public TFilterMixin<TRefView<TRng>>
        , public TToMixin<TRefView<TRng>>
        , public TMinMaxMixin<TRefView<TRng>>
        , public TFindMixin<TRefView<TRng>>
        , public TConditionalInheritance<BiDirRange<TRng>, TFindLastMixin<TRefView<TRng>>>
        , public TIteratorMixin<TRefView<TRng>>
        , public TConditionalInheritance<BiDirRange<TRng>, TReverseMixin<TRefView<TRng>>>
        , public TConditionalInheritance<BiDirRange<TRng>, TReverseIteratorMixin<TRefView<TRng>>>
    {
        friend struct FCursorProtocol;

    public:
        using value_type = RangeValue<TRng>;
        using reference = RangeReference<TRng>;
        using const_reference = RangeConstReference<TRng>;

        using BaseRange = TRng;
        using Cursor = RangeIterator<TRng>;
        using ReverseCursor = Detail::TReverseCursorType<BiDirRange<TRng>, TRng>;

        static constexpr bool IsBidir = BiDirRange<TRng>;

        template<typename URng>
        TRefView(Misc::FFromViewTag, URng& InRng)
            : Rng(std::addressof(InRng))
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TFn>
        UR_DEBUG_NOINLINE static void InternalIteration(TSelf& Self, TFn Fn)
        {
            for (auto It = Ur::BeginEx<IsForward>(*Self.Rng); It != Ur::EndEx<IsForward>(*Self.Rng); ++It)
                if (Fn(*It) == Misc::ELoop::Break)
                    break;
        }

        UR_DEBUG_NOINLINE Cursor CursorBegin() const
        {
            return Ur::Begin(*Rng);
        }

        UR_DEBUG_NOINLINE Cursor CursorEnd() const
        {
            return Ur::End(*Rng);
        }

        UR_DEBUG_NOINLINE ReverseCursor CursorRBegin() const requires IsBidir
        {
            return Ur::RBegin(*Rng);
        }

        UR_DEBUG_NOINLINE ReverseCursor CursorREnd() const requires IsBidir
        {
            return Ur::REnd(*Rng);
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE void CursorInc(TCursor& Curs) const
        {
            ++Curs;
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE reference CursorDeref(const TCursor& Curs) const
        {
            return *Curs;
        }

        template<typename TCursor>
        UR_DEBUG_NOINLINE bool CursorEq(const TCursor& Lhs, const TCursor& Rhs) const
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
