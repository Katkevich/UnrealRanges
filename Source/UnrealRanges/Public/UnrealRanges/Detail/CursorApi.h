#pragma once

namespace Ur {
struct FCursorApi
{
    template<typename TView, typename TFn>
    static decltype(auto) InternalIteration(TView& View, TFn Fn)
    {
        return View.InternalIteration(Fn);
    }

    template<typename TView>
    static decltype(auto) CursorBegin(TView& View)
    {
        return View.CursorBegin();
    }

    template<typename TView>
    static decltype(auto) CursorEnd(TView& View)
    {
        return View.CursorEnd();
    }

    template<typename TView, typename TCursor>
    static decltype(auto) CursorInc(TView& View, TCursor& Curs)
    {
        return View.CursorInc(Curs);
    }

    template<typename TView, typename TCursor>
    static decltype(auto) CursorDeref(TView& View, const TCursor& Curs)
    {
        return View.CursorDeref(Curs);
    }

    template<typename TView, typename TCursor>
    static bool CursorEq(TView& View, const TCursor& Lhs, const TCursor& Rhs)
    {
        return View.CursorEq(Lhs, Rhs);
    }
};
}