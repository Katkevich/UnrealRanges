#pragma once
#include "UnrealRanges/Detail/AlwaysFalse.h"
#include "UnrealRanges/Utility.h"

namespace Ur {
    namespace View {
        template<typename TView, bool IsForward>
        struct TViewCursor;

        template<typename TView>
        struct TViewCursor<TView, true>
        {
            using Type = typename TView::Cursor;
        };

        template<typename TView>
        struct TViewCursor<TView, false>
        {
            using Type = typename TView::ReverseCursor;
        };

        template<typename TView>
        struct TViewCursor<const TView, true>
        {
            using Type = typename TView::ConstCursor;
        };

        template<typename TView>
        struct TViewCursor<const TView, false>
        {
            using Type = typename TView::ReverseConstCursor;
        };

        template<typename TView, bool IsForward>
        using TCursor = typename TViewCursor<TView, IsForward>::Type;



        template<typename TView>
        struct TViewReference
        {
            using Type = typename TView::reference;
        };

        template<typename TView>
        struct TViewReference<const TView>
        {
            using Type = typename TView::const_reference;
        };

        template<typename TView>
        using TReference = typename TViewReference<TView>::Type;
    }

    struct FCursorProtocol
    {
        template<bool IsForward, typename TView, typename TFn>
        static decltype(auto) InternalIteration(TView& View, TFn Fn)
        {
            return TView::InternalIteration<IsForward>(View, Fn);
        }

        template<bool IsForward, typename TView>
        static decltype(auto) CursorBegin(TView& View)
        {
            return TView::CursorBegin<IsForward>(View);
        }

        template<bool IsForward, typename TView>
        static decltype(auto) CursorEnd(TView& View)
        {
            return TView::CursorEnd<IsForward>(View);
        }

        template<typename TView, typename TCursor>
        static decltype(auto) CursorInc(TView& View, TCursor& Curs)
        {
            return TView::CursorInc(View, Curs);
        }

        template<typename TView, typename TCursor>
        static decltype(auto) CursorDeref(TView& View, const TCursor& Curs)
        {
            return TView::CursorDeref(View, Curs);
        }

        template<typename TView, typename TCursor>
        static bool CursorEq(TView& View, const TCursor& Lhs, const TCursor& Rhs)
        {
            return TView::CursorEq(View, Lhs, Rhs);
        }


        template<typename TView, typename TCursor>
        static constexpr bool IsForwardCursor =
            std::is_same_v<TCursor, typename TView::Cursor> ||
            std::is_same_v<TCursor, typename TView::ConstCursor>;

        template<typename TView, typename TCursor>
        static bool IsEnd(TView& View, const TCursor& Curs)
        {
            return TView::CursorEq(View, Curs, TView::CursorEnd<IsForwardCursor<TView, TCursor>>(View));
        }

        template<typename TView>
        static auto BaseViewNum(const TView& View)
        {
            return Ur::Size(View.View);
        }
    };
}