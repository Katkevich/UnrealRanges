#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "UnrealRanges/View/TransformView.h"
#include "UnrealRanges/Fn/Pair.h"

namespace Ur::View {
    template<Ur::RangeView TUnderlView>
    class TValuesView : public TTransformView<TUnderlView, Ur::Fn::FValue>
    {
    public:
        template<typename UUnderlView>
        TValuesView(Misc::FFromViewTag, UUnderlView&& InView)
            : TTransformView<TUnderlView, Ur::Fn::FValue>(UR_FWD(InView), Ur::Fn::Value)
        {
        }
    };


    template<typename TRng>
    auto Values(TRng& Rng)
    {
        //TODO: own view & view in support
        return TValuesView<TRefView<TRng>>({}, TRefView<TRng>({}, Rng));
    }
}