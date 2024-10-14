#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include "UnrealRanges/Optional.h"

namespace Ur::View {

    template<typename TBaseGroupByView, typename TBaseCursor>
    class TGroupView
        : public FView
        , public Detail::TMixins<TGroupView<TBaseGroupByView, TBaseCursor>, TDefaultMixins>
        , public Detail::TConditionalMixins<TIsPair_V<typename TBaseGroupByView::TUnderlyingView::value_type>, TGroupView<TBaseGroupByView, TBaseCursor>, TMapMixins>
    {
        friend struct Ur::Cursor;

        static constexpr bool IsNestedCursorConst = std::is_same_v<TBaseCursor, typename TBaseGroupByView::ConstCursor>;

    public:
        using reference = std::conditional_t<
            IsNestedCursorConst,
            typename TBaseGroupByView::TUnderlyingView::const_reference,
            typename TBaseGroupByView::TUnderlyingView::reference
        >;
        using const_reference = typename TBaseGroupByView::TUnderlyingView::const_reference;
        using value_type = typename TBaseGroupByView::TUnderlyingView::value_type;

        static_assert(std::is_convertible_v<reference, const_reference>,
            "GroupView uses a hack when it may use non-const underlying cursor (Nested cursor) inside the ConstCursor"
            "(you created a TGroupView from non-const TGroupByView (it will store non-const cursor inside in this case) "
            "and then called const method on this TGroupView). "
            "In roder to preserve const-correctness we may need to be able to convert these non-const references to const ones");

        struct Cursor
        {
            // "empty" means "end" cursor
            TOptional<TBaseCursor> Nested{};
        };

        struct ConstCursor
        {
            // "empty" means "end" cursor
            TOptional<TBaseCursor> Nested{};
        };
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = false;
        static constexpr bool LikeMap = TIsPair_V<value_type>;

        template<typename UBaseGroupByView, typename UBaseCursor>
        TGroupView(const UBaseGroupByView& InGroupByView, const UBaseCursor& InBeginCursor)
            : GroupByView(InGroupByView)
            , BeginCursor(InBeginCursor)
        {
        }

        decltype(auto) GroupKey() const
        {
            return TBaseGroupByView::EvalGroupKey(AdjustConstness(GroupByView), BeginCursor);
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            // no optimal implementation using only internal iteration because GroupView is iterating
            // underlying view FROM A PARTICULAR cursor (from Self.BeginCursor) and not from the beginning of the underlying view
            // so fallback to default cursor-based iteration instead
            auto EndCurs = CursorEnd<IsForward>(Self);
            for (auto Curs = CursorBegin<IsForward>(Self); !CursorEq(Self, Curs, EndCurs); CursorInc(Self, Curs))
            {
                if (Callback(CursorDeref(Self, Curs)) == Ur::ELoop::Break)
                    break;
            }
            return Ur::ELoop::Break;
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            // NOTE: both const and non-const cursors may contain non-const underlying cursor
            // which may require const_cast hack (see AdjustConstness)
            // but this should be safe coz CursorDeref will return const-correct version of reference anyway
            // (non-const reference after non-const nested cursor dereferencing will be converted to const reference if required)
            return TCursor<TSelf, IsForward>{ Self.BeginCursor };
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return TCursor<TSelf, IsForward>{ NullOpt };
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            decltype(auto) AdjustedGroupByView = AdjustConstness(Self.GroupByView);
            decltype(auto) PreviousGroupKey = TBaseGroupByView::EvalGroupKey(AdjustedGroupByView, *Curs.Nested);

            Ur::Cursor::Inc(AdjustedGroupByView.View, *Curs.Nested);

            if (Ur::Cursor::IsEnd(AdjustedGroupByView.View, *Curs.Nested) || !(PreviousGroupKey == TBaseGroupByView::EvalGroupKey(AdjustedGroupByView, *Curs.Nested)))
                Curs.Nested.Reset();
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static auto CursorDeref(TSelf& Self, const TCursor& Curs) -> TReference<TSelf>
        {
            return Ur::Cursor::Deref(AdjustConstness(Self.GroupByView).View, *Curs.Nested);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return Lhs.Nested.IsSet() == Rhs.Nested.IsSet() && (!Lhs.Nested.IsSet() || Ur::Cursor::Eq(AdjustConstness(Self.GroupByView), *Lhs.Nested, *Rhs.Nested));
        }


        template<typename UBaseGroupByView>
        static decltype(auto) AdjustConstness(UBaseGroupByView& GroupByView)
        {
            if constexpr (IsNestedCursorConst)
            {
                return static_cast<std::add_const_t<TBaseGroupByView>&>(GroupByView);
            }
            else
            {
                // proper iterator implementation should allow "iterator to const_iterator" implicit convertion (std iterators)
                // but many UE iterators cannot do that which means we cannot convert Cursor into ConstCursor (they are basically wrappers around iterators)
                // thus we must use const_cast hacks here (non-const cursor should only be working with non-const view and vise-versa)
                // 
                // NOTE: even tho we use const_cast hack here const-correctness is 100% preserved (non-const references are being casted to const-references in CursorDeref)
                return const_cast<std::remove_const_t<TBaseGroupByView>&>(GroupByView);
            }
        }

        template<typename TSelf, typename TCursor>
        static decltype(auto) EvalGroupKey(TSelf& Self, TCursor& NestedCursor)
        {
            return std::invoke(
                AdjustConstness(Self.GroupByView).Fn,
                Ur::Cursor::Deref(AdjustConstness(Self.GroupByView).View, NestedCursor));
        }

    private:
        TBaseGroupByView GroupByView{};
        TBaseCursor BeginCursor{};
    };



    template<Ur::RangeView TUnderlView, std::regular_invocable<typename TUnderlView::reference> TFn>
    class TGroupByView
        : public FView
        , public Detail::TMixins<TGroupByView<TUnderlView, TFn>, TDefaultMixins>
    {
        friend struct Ur::Cursor;

        template<typename TBaseGroupByView, typename TBaseCursor>
        friend class TGroupView;

        using TUnderlyingView = TUnderlView;

    public:
        using Cursor = typename TUnderlView::Cursor;
        using ConstCursor = typename TUnderlView::ConstCursor;
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        using reference = TGroupView<TGroupByView<TUnderlView, TFn>, Cursor>;
        using const_reference = TGroupView<TGroupByView<TUnderlView, TFn>, ConstCursor>;
        using value_type = reference;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = false;
        static constexpr bool LikeMap = false;

        static_assert(std::is_invocable_v<TFn, typename TUnderlView::reference>, "TFn should be invokable over underlying view items references");
        static_assert(EqComparable<std::invoke_result_t<TFn, typename TUnderlView::reference>>, "The result of TFn invokation should be comparable using '==' operator");

        template<typename UUnderlView, typename UFn>
        TGroupByView(UUnderlView&& InView, UFn&& InFn)
            : View(UR_FWD(InView))
            , Fn(UR_FWD(InFn))
        {
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            // no optimal implementation using only internal iteration because GroupView requires cursor for it to work anyway
            // so fallback to default cursor-based iteration instead
            auto EndCurs = CursorEnd<IsForward>(Self);
            for (auto Curs = CursorBegin<IsForward>(Self); !CursorEq(Self, Curs, EndCurs); CursorInc(Self, Curs))
            {
                if (Callback(CursorDeref(Self, Curs)) == Ur::ELoop::Break)
                    break;
            }
            return Ur::ELoop::Break;
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return Ur::Cursor::Begin<IsForward>(Self.View);
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return Ur::Cursor::End<IsForward>(Self.View);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            decltype(auto) PreviousGroupKey = EvalGroupKey(Self, Curs);

            Ur::Cursor::Inc(Self.View, Curs);

            // fast-forward till end of the whole underlying view or end of the group
            while (!Ur::Cursor::IsEnd(Self.View, Curs) && PreviousGroupKey == EvalGroupKey(Self, Curs))
                Ur::Cursor::Inc(Self.View, Curs);
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return TReference<TSelf>{ Self, Curs };
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            return Ur::Cursor::Eq(Self.View, Lhs, Rhs);
        }

        template<typename TSelf, typename TCursor>
        static decltype(auto) EvalGroupKey(TSelf& Self, TCursor& Cursor)
        {
            return std::invoke(Self.Fn, Ur::Cursor::Deref(Self.View, Cursor));
        }

    private:
        TUnderlView View;
        TFn Fn{};
    };


    template<typename TRng, typename TFn = FIdentityFunctor>
    auto GroupBy(TRng& Rng, TFn&& Fn = {})
    {
        //TODO: own view & view in support
        return TGroupByView<TRefView<TRng>, std::decay_t<TFn>>(TRefView<TRng>({}, Rng), UR_FWD(Fn));
    }
}