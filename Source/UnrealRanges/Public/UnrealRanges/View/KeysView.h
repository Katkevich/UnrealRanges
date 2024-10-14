#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "UnrealRanges/View/TransformView.h"
#include "UnrealRanges/Fn/Pair.h"

namespace Ur::View {
    template<Ur::RangeView TUnderlView>
    class TKeysView : public TTransformView<TUnderlView, Ur::Fn::FKey>
    {
    public:
        template<typename UUnderlView>
        TKeysView(Misc::FFromViewTag, UUnderlView&& InView)
            : TTransformView<TUnderlView, Ur::Fn::FKey>(UR_FWD(InView), Ur::Fn::Key)
        {
        }
    };


    template<typename TRng>
    auto Keys(TRng& Rng)
    {
        //TODO: own view & view in support
        return TKeysView<TRefView<TRng>>({}, TRefView<TRng>({}, Rng));
    }
}