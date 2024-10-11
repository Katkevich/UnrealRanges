#pragma once
#include "UnrealRanges/View/View.h"
#include "UnrealRanges/Utility.h"
#include "Templates/IntegralConstant.h"
#include "Templates/Tuple.h"
#include <concepts>
#include <type_traits>
#include <cstddef>

namespace Ur {

    template<typename TRng>
    concept Range = HasBegin<TRng> && HasEnd<TRng>;

    template<typename TRng>
    concept BiDirRange = Range<TRng> && HasRBegin<TRng> && HasREnd<TRng>;

    template<typename TIt>
    struct IteratorTraits
    {
        // we cannot rely on iterator::reference coz UE containers may not provide it
        using reference = decltype(*std::declval<std::add_lvalue_reference_t<TIt>>()); // TIt& && -> TIt&
        using value_type = std::remove_cvref_t<reference>;
        using pointer = std::conditional_t<std::is_lvalue_reference_v<reference>, std::add_pointer_t<std::remove_reference_t<reference>>, void>;
    };

    template<typename TIt>
    using IteratorValue = typename IteratorTraits<TIt>::value_type;

    template<typename TIt>
    using IteratorReference = typename IteratorTraits<TIt>::reference;

    template<typename TIt>
    using IteratorPointer = typename IteratorTraits<TIt>::pointer;


    // we cannot rely on TRng::iterator or something similar coz UE containers may not provide it consistently
    template<Ur::Range TRng>
    using RangeIterator = decltype(Ur::Begin(std::declval<std::add_lvalue_reference_t<TRng>>())); // TRng& && -> TRng&

    template<Ur::Range TRng>
    using RangeConstIterator = decltype(Ur::Begin(std::declval<std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<TRng>>>>())); // const TRng& && -> const TRng&

    template<BiDirRange TRng>
    using RangeReverseIterator = decltype(Ur::RBegin(std::declval<std::add_lvalue_reference_t<TRng>>())); // TRng& && -> TRng&

    template<BiDirRange TRng>
    using RangeReverseConstIterator = decltype(Ur::RBegin(std::declval<std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<TRng>>>>())); // const TRng& && -> const TRng&

    template<Ur::Range TRng>
    using RangeValue = IteratorValue<RangeIterator<TRng>>;

    template<Ur::Range TRng>
    using RangeReference = IteratorReference<RangeIterator<TRng>>;

    template<Ur::Range TRng>
    using RangeConstReference = IteratorReference<RangeConstIterator<TRng>>;

    template<Ur::Range TRng>
    using RangePointer = IteratorPointer<RangeIterator<TRng>>;

    template<Ur::Range TRng>
    using RangeConstPointer = IteratorPointer<RangeConstIterator<TRng>>;


    template<typename TRng>
    concept SizedRange = Range<TRng> && requires(TRng Rng) {
        { Ur::Size(Rng) } -> std::convertible_to<int32>;
    };

    // UE random "accessness" is very limited (basically the only facility that is available is operator[])
    template<typename TRng>
    concept RandomAccessRange = SizedRange<TRng> && requires(TRng Rng, int32 Index) {
        { Rng[Index] } -> std::same_as<RangeReference<TRng>>;
    };

    template<typename TRng>
    concept ContiguousRange = RandomAccessRange<TRng> && requires(TRng Rng) {
        { Ur::Data(Rng) } -> std::same_as<RangePointer<TRng>>;
    };

    template<typename TView>
    concept RangeView = std::derived_from<TView, Ur::View::FView>;

    template<typename TFn, typename TView>
    concept ViewPredicate = requires(TFn Fn, typename TView::reference Item) {
        { std::invoke(Fn, Item) } -> std::convertible_to<bool>;
    };

    // TTupleElement
    namespace Detail {
        template<std::size_t Idx, typename T>
        struct TTupleElement
        {
            using Type = std::tuple_element_t<Idx, T>;
        };

        template<std::size_t Idx, typename... Ts>
        struct TTupleElement<Idx, TTuple<Ts...>>
        {
            using Type = typename ::TTupleElement<Idx, TTuple<Ts...>>::Type;
        };
    }

    template<std::size_t Idx, typename T>
    struct TTupleElement
    {
        using Type = typename Detail::TTupleElement<Idx, std::remove_cvref_t<T>>::Type;
    };

    template<std::size_t Idx, typename T>
    using TTupleElement_T = typename TTupleElement<Idx, T>::Type;


    // TTupleSize
    namespace Detail {
        template<typename T>
        struct TTupleSize
        {
            static constexpr std::size_t Value = std::tuple_size_v<T>;
        };

        template<typename... Ts>
        struct TTupleSize<TTuple<Ts...>>
        {
            static constexpr std::size_t Value = sizeof...(Ts);
        };
    }

    template<typename T>
    struct TTupleSize
    {
        static constexpr std::size_t Value = Detail::TTupleSize<std::remove_cvref_t<T>>::Value;
    };

    template<typename T>
    static constexpr std::size_t TTupleSize_V = TTupleSize<T>::Value;


    //// TTupleProtocol
    //namespace Detail {
    //    template<typename TTupleLike, std::size_t... Is>
    //    concept TupleProtocol = requires(TTupleLike Tpl) {
    //        TTupeList<TTupleElement_T<Is, TTupleLike>...>{};
    //        (get<Is>(Tpl), ...);
    //    };

    //    template<typename TTupleLike, typename TIndexSeq>
    //    constexpr bool TupleProtocol_V = false;

    //    template<typename TTupleLike, std::size_t... Is>
    //    constexpr bool TupleProtocol_V<TTupleLike, std::index_sequence<Is...>> = TupleProtocol<TTupleLike, Is...>;
    //}

    //template<typename TTupleLike>
    //concept TupleProtocol = Detail::TupleProtocol_V<TTupleLike, std::make_index_sequence<TTupleSize_V<TTupleLike>>>;


    // TIsSameTemplate
    template<template<typename...> typename T, template<typename...> typename U>
    struct TIsSameTemplate : TIntegralConstant<bool, false>
    {};

    template<template<typename...> typename T>
    struct TIsSameTemplate<T, T> : TIntegralConstant<bool, true>
    {};

    template<template<typename...> typename T, template<typename...> typename U>
    static constexpr bool TIsSameTemplate_V = TIsSameTemplate<T, U>::Value;


    // THasTemplate
    namespace Detail
    {
        template<typename T, template<typename...> typename Tmpl>
        struct THasTemplate : TIntegralConstant<bool, false>
        {};

        template<template<typename...> typename Tmpl, typename... Ts>
        struct THasTemplate<Tmpl<Ts...>, Tmpl> : TIntegralConstant<bool, true>
        {};
    }

    template<typename T, template<typename...> typename Tmpl>
    struct THasTemplate : Detail::THasTemplate<std::remove_cvref_t<T>, Tmpl>
    {};

    template<typename T, template<typename...> typename Tmpl>
    static constexpr bool THasTemplate_V = THasTemplate<T, Tmpl>::Value;



    template<typename TLhs, typename TRhs = TLhs>
    concept LessOrdered = requires(TLhs Lhs, TRhs Rhs) {
        { Lhs < Rhs } -> std::convertible_to<bool>;
    };

    template<typename T, typename TProj>
    concept LessOrderedBy = requires(T Val, TProj Proj) {
        { std::invoke(Proj, Val) < std::invoke(Proj, Val) } -> std::convertible_to<bool>;
    };

    template<typename TLhs, typename TRhs = TLhs>
    concept EqComparable = requires(TLhs Lhs, TRhs Rhs) {
        { Lhs == Rhs } -> std::convertible_to<bool>;
    };

    template<typename TLhs, typename TProj, typename TRhs>
    concept EqComparableByWith = requires(TLhs Lhs, TProj Proj, TRhs Rhs) {
        { std::invoke(Proj, Lhs) == Rhs } -> std::convertible_to<bool>;
    };
}