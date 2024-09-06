#pragma once
#include "UnrealRanges/View/Mixin/Transform.h"
#include "UnrealRanges/View/Mixin/Iterator.h"
#include "UnrealRanges/View/View.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "UnrealRanges/Detail/CursorApi.h"
#include <memory>

namespace Ur::View {

    template<typename TRng>
    class TRefView
        : public FView
        , public TIteratorMixin<TRefView<TRng>>
    {
        friend struct FCursorApi;

    public:
        using value_type = RangeValue<TRng>;
        using reference = RangeReference<TRng>;
        using const_reference = RangeConstReference<TRng>;

        using Cursor = RangeIterator<TRng>;

        template<typename URng>
        TRefView(FFromViewTag, URng& InRng)
        {
            Rng = std::addressof(InRng);
        }

    private:
        template<typename TFn>
        void InternalIteration(TFn Fn)
        {
            for (auto&& Item : *Rng)
            {
                if (Fn(UR_FWD(Item)) == ELoop::Break)
                {
                    break;
                }
            }
        }

        Cursor CursorBegin() const
        {
            return Ur::Begin(*Rng);
        }

        Cursor CursorEnd() const
        {
            return Ur::End(*Rng);
        }

        void CursorInc(Cursor& Curs) const
        {
            ++Curs;
        }

        reference CursorDeref(const Cursor& Curs) const
        {
            return *Curs;
        }

        bool CursorEq(const Cursor& Lhs, const Cursor& Rhs) const
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
