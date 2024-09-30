#pragma once
#include "UnrealRanges/Detail/ForwardMacro.h"
#include "UnrealRanges/View/TransformView.h"
#include "UnrealRanges/Fn/Pair.h"

namespace Ur::View {
    template<typename TView>
    class TKeysView : public TTransformView<TView, Ur::Fn::FKey>
    {
    public:
        template<typename UView>
        TKeysView(Misc::FFromViewTag, UView&& InView)
            : TTransformView<TView, Ur::Fn::FKey>(UR_FWD(InView), Ur::Fn::Key)
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