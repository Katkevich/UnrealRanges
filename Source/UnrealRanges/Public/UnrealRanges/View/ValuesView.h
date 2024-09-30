#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "UnrealRanges/View/TransformView.h"
#include "UnrealRanges/Fn/Pair.h"

namespace Ur::View {
    template<typename TView>
    class TValuesView : public TTransformView<TView, Ur::Fn::FValue>
    {
    public:
        template<typename UView>
        TValuesView(Misc::FFromViewTag, UView&& InView)
            : TTransformView<TView, Ur::Fn::FValue>(UR_FWD(InView), Ur::Fn::Value)
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