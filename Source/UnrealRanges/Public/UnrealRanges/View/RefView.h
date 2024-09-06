#pragma once
#include "UnrealRanges/View/Mixin/Transform.h"
#include "UnrealRanges/View/Mixin/Reverse.h"
#include "UnrealRanges/View/Mixin/Iterator.h"
#include "UnrealRanges/View/AlgoMixin/To.h"
#include "UnrealRanges/View/View.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "UnrealRanges/Detail/EmptyBase.h"
#include "UnrealRanges/Detail/CursorProtocol.h"
#include <memory>

namespace Ur::View {
    namespace Detail {
        template<bool IsBiDir, typename TRng>
        struct TReverseCursor
        {
            using Type = void;
        };

        template<typename TRng>
        struct TReverseCursor<true, TRng>
        {
            using Type = RangeReverseIterator<TRng>;
        };

        template<bool IsBiDir, typename TRng>
        using TReverseCursorType = typename TReverseCursor<IsBiDir, TRng>::Type;
    }

    template<typename TRng>
    class TRefView
        : public FView
        , public TIteratorMixin<TRefView<TRng>>
        , public TToMixin<TRefView<TRng>>
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

        static constexpr bool IsBiDir = BiDirRange<TRng>;

        template<typename URng>
        TRefView(Misc::FFromViewTag, URng& InRng)
            : Rng(std::addressof(InRng))
        {
        }

    private:
        template<bool Direction, typename TFn>
        void InternalIteration(TFn Fn)
        {
            for (auto It = Ur::BeginEx<Direction>(*Rng); It != Ur::EndEx<Direction>(*Rng); ++It)
                if (Fn(*It) == Misc::ELoop::Break)
                    break;
        }

        Cursor CursorBegin() const
        {
            return Ur::Begin(*Rng);
        }

        Cursor CursorEnd() const
        {
            return Ur::End(*Rng);
        }

        ReverseCursor CursorRBegin() const requires BiDirRange<TRng>
        {
            return Ur::RBegin(*Rng);
        }

        ReverseCursor CursorREnd() const requires BiDirRange<TRng>
        {
            return Ur::REnd(*Rng);
        }

        template<typename TCursor>
        void CursorInc(TCursor& Curs) const
        {
            ++Curs;
        }

        template<typename TCursor>
        reference CursorDeref(const TCursor& Curs) const
        {
            return *Curs;
        }

        template<typename TCursor>
        bool CursorEq(const TCursor& Lhs, const TCursor& Rhs) const
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
