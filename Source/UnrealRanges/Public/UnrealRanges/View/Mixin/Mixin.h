#pragma once
#include "UnrealRanges/View/Mixin/Transform.h"
#include "UnrealRanges/View/Mixin/Filter.h"
#include "UnrealRanges/View/Mixin/Enumerate.h"
#include "UnrealRanges/View/Mixin/Take.h"
#include "UnrealRanges/View/Mixin/TakeWhile.h"
#include "UnrealRanges/View/Mixin/Drop.h"
#include "UnrealRanges/View/Mixin/DropWhile.h"
#include "UnrealRanges/View/Mixin/GroupBy.h"
#include "UnrealRanges/View/Mixin/Concat.h"
#include "UnrealRanges/View/Mixin/Zip.h"
#include "UnrealRanges/View/Mixin/Keys.h"
#include "UnrealRanges/View/Mixin/Values.h"
#include "UnrealRanges/View/Mixin/Adjacent.h"
#include "UnrealRanges/View/Mixin/Swizzle.h"
#include "UnrealRanges/View/Mixin/Reverse.h"
#include "UnrealRanges/View/Mixin/Iterator.h"
#include "UnrealRanges/View/AlgoMixin/To.h"
#include "UnrealRanges/View/AlgoMixin/MinMax.h"
#include "UnrealRanges/View/AlgoMixin/Find.h"
#include "UnrealRanges/View/AlgoMixin/Size.h"
#include "UnrealRanges/View/AlgoMixin/Count.h"
#include "UnrealRanges/View/AlgoMixin/IsEmpty.h"
#include "UnrealRanges/View/AlgoMixin/Fold.h"
#include "UnrealRanges/View/AlgoMixin/Sum.h"
#include "UnrealRanges/View/AlgoMixin/Foreach.h"
#include "UnrealRanges/View/AlgoMixin/EqualTo.h"
#include "UnrealRanges/View/AlgoMixin/StartsWith.h"
#include "UnrealRanges/View/AlgoMixin/Front.h"
#include "UnrealRanges/View/AlgoMixin/Back.h"
#include <type_traits>

namespace Ur::View {
    namespace Detail {
        template<template<typename...> typename... Ts>
        struct FEmptyBase
        {
        };

        template<template<typename> typename... Ts>
        struct TMixinList;


        template<typename TView, typename UMixinList>
        struct TMixins;

        template<typename TView, template<typename> typename... UMixins>
        struct TMixins<TView, TMixinList<UMixins...>> : public UMixins<TView>...
        {};


        template<bool bCondition, typename TView, typename UMixins>
        struct TConditionalMixins;

        template<typename TView, template<typename> typename... UMixins>
        struct TConditionalMixins<true, TView, TMixinList<UMixins...>> : public UMixins<TView>...
        {};

        template<typename TView, template<typename> typename... UMixins>
        struct TConditionalMixins<false, TView, TMixinList<UMixins...>> : public Detail::FEmptyBase<UMixins...>
        {};
    }

    using TDefaultMixins = Detail::TMixinList<
        TTransformMixin,
        TFilterMixin,
        TEnumerateMixin,
        TTakeMixin,
        TTakeWhileMixin,
        TDropMixin,
        TDropWhileMixin,
        TGroupByMixin,
        TConcatMixin,
        TZipMixin,
        TToMixin,
        TMinMaxMixin,
        TFindFirstMixin,
        TCountMixin,
        TIsEmptyMixin,
        TFoldLeftMixin,
        TSumMixin,
        TForeachMixin,
        TEqualToMixin,
        TStartsWithMixin,
        TFrontMixin,
        TAdjacentMixin,
        TSwizzleMixin,
        TIteratorMixin
    >;

    using TBidirMixins = Detail::TMixinList<
        TFindLastMixin,
        TFoldRightMixin,
        TBackMixin,
        TReverseMixin,
        TReverseIteratorMixin
    >;

    using TSizedMixins = Detail::TMixinList<
        TSizeMixin
    >;

    using TMapMixins = Detail::TMixinList<
        TKeysMixin,
        TValuesMixin
    >;

}