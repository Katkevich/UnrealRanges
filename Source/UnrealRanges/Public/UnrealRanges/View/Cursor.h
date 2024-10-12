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

    struct Cursor
    {
        template<bool IsForward = true, typename TView, typename TFn>
        static decltype(auto) Iterate(TView& View, TFn Fn)
        {
            return TView::template InternalIterate<IsForward>(View, Fn);
        }

        template<bool IsForward = true, typename TView>
        static decltype(auto) Begin(TView& View)
        {
            return TView::template CursorBegin<IsForward>(View);
        }

        template<bool IsForward = true, typename TView>
        static decltype(auto) End(TView& View)
        {
            return TView::template CursorEnd<IsForward>(View);
        }

        template<typename TView, typename TCursor>
        static decltype(auto) Inc(TView& View, TCursor& Curs)
        {
            return TView::template CursorInc(View, Curs);
        }

        template<typename TView, typename TCursor>
        static decltype(auto) Deref(TView& View, const TCursor& Curs)
        {
            return TView::template CursorDeref(View, Curs);
        }

        template<typename TView, typename TCursor>
        static bool Eq(TView& View, const TCursor& Lhs, const TCursor& Rhs)
        {
            return TView::template CursorEq(View, Lhs, Rhs);
        }


        template<typename TView, typename TCursor>
        static constexpr bool IsForward =
            std::is_same_v<TCursor, typename TView::Cursor> ||
            std::is_same_v<TCursor, typename TView::ConstCursor>;

        template<typename TView, typename TCursor>
        static bool IsEnd(TView& View, const TCursor& Curs)
        {
            return TView::template CursorEq(View, Curs, TView::template CursorEnd<IsForward<TView, TCursor>>(View));
        }

        template<typename TView>
        static auto BaseViewNum(const TView& View)
        {
            return Ur::Size(View.View);
        }
    };
}