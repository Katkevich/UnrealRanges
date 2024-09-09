#pragma once
#include "UnrealRanges/Utility.h"

namespace Ur {
struct FCursorProtocol
{
    template<bool IsForward, typename TView, typename TFn>
    static decltype(auto) InternalIteration(Misc::FDirection<IsForward>, TView& View, TFn Fn)
    {
        return TView::InternalIteration<IsForward, TView, TFn>(View, Fn);
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

    template<typename TView>
    static decltype(auto) CursorRBegin(TView& View)
    {
        return View.CursorRBegin();
    }

    template<typename TView>
    static decltype(auto) CursorREnd(TView& View)
    {
        return View.CursorREnd();
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