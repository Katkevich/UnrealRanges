#pragma once
#include "UnrealRanges/Detail/CursorProtocol.h"
#include "UnrealRanges/Detail/EmptyBase.h"
#include "UnrealRanges/Optional.h"

namespace Ur::View {

    namespace Detail
    {
        template<typename TView>
        struct TFoldMixin
        {
        protected:
            template<typename TRef, typename TInit, typename TFn, typename TSelf, bool IsForward>
            static auto Fold(std::type_identity<TRef>, TInit&& Init, TFn Fn, TSelf& Self, Misc::FDirection<IsForward> Direction)
            {
                static_assert(std::is_invocable_v<TFn, TInit, TRef>, "Fn should be invokable over 'Init <-> Item'");

                using ResultType = std::invoke_result_t<TFn, TInit, TRef>;
                static_assert(std::is_invocable_r_v<ResultType, TFn, ResultType, TRef>, "Fn should be invokable over 'Fn(Init, Item) <-> Item')");

                static_assert(!std::is_reference_v<ResultType>, "cannot be a reference");

                TOptional<ResultType> Result;

                FCursorProtocol::InternalIteration(Direction, Self, [&](auto&& Item)
                    {
                        if (Result)
                            Result.Emplace(std::invoke(Fn, MoveTemp(*Result), UR_FWD(Item)));
                        else
                            Result.Emplace(std::invoke(Fn, UR_FWD(Init), UR_FWD(Item)));

                        return Misc::ELoop::Continue;
                    });

                if (Result)
                    return MoveTemp(*Result);
                else
                    return ResultType(UR_FWD(Init));
            }

            template<typename TRef, typename TFn, typename TSelf, bool IsForward>
            static auto Fold(std::type_identity<TRef>, TFn Fn, TSelf& Self, Misc::FDirection<IsForward> Direction)
            {
                static_assert(std::is_invocable_v<TFn, TRef, TRef>, "Fn should be invokable over 'Item <-> Item'");
                using ResultType = std::invoke_result_t<TFn, TRef, TRef>;

                static_assert(std::is_invocable_r_v<ResultType, TFn, ResultType, TRef>, "Fn should be invokable over 'Fn(Item, Item) <-> Item')");

                static_assert(!std::is_reference_v<ResultType>, "cannot be a reference");
                static_assert(std::is_reference_v<TRef> || std::move_constructible<TRef>, "range item should be either a reference or a movable value");

                TOptional<ResultType> Result;
                // we have specialization for TOptional<T&> so TRef being a reference is not a problem
                TOptional<TRef> FstItem;

                // even tho we are going through all these hoops using TOptional it is still better than 
                // multiple invocation of TransformView predicate while using iterators
                FCursorProtocol::InternalIteration(Direction, Self, [&](auto&& Item)
                    {
                        if (Result)
                        {
                            Result.Emplace(std::invoke(Fn, MoveTemp(*Result), UR_FWD(Item)));
                        }
                        else if (!FstItem)
                        {
                            FstItem.Emplace(UR_FWD(Item));
                        }
                        else
                        {
                            if constexpr (std::is_reference_v<TRef>)
                                Result.Emplace(std::invoke(Fn, *FstItem, UR_FWD(Item)));
                            else
                                Result.Emplace(std::invoke(Fn, MoveTemp(*FstItem), UR_FWD(Item)));
                        }

                        return Misc::ELoop::Continue;
                    });

                return Result;
            }
        };
    }

    template<typename TView>
    struct TFoldLeftMixin : Detail::TFoldMixin<TView>
    {
        using TFoldMixin = Detail::TFoldMixin<TView>;

        template<typename TInit, typename TFn>
        auto FoldLeft(TInit&& Init, TFn Fn)
        {
            return TFoldMixin::Fold(std::type_identity<typename TView::reference>{}, UR_FWD(Init), Fn, * static_cast<TView*>(this), Misc::Forward);
        }

        template<typename TInit, typename TFn>
        auto FoldLeft(TInit&& Init, TFn Fn) const
        {
            return TFoldMixin::Fold(std::type_identity<typename TView::const_reference>{}, UR_FWD(Init), Fn, * static_cast<const TView*>(this), Misc::Forward);
        }

        template<typename TFn>
        auto FoldLeft(TFn Fn)
        {
            return TFoldMixin::Fold(std::type_identity<typename TView::reference>{}, Fn, *static_cast<TView*>(this), Misc::Forward);
        }

        template<typename TFn>
        auto FoldLeft(TFn Fn) const
        {
            return TFoldMixin::Fold(std::type_identity<typename TView::const_reference>{}, Fn, *static_cast<const TView*>(this), Misc::Forward);
        }
    };

    template<typename TView>
    struct TFoldRightMixin : Detail::TFoldMixin<TView>
    {
        using TFoldMixin = Detail::TFoldMixin<TView>;

        template<typename TInit, typename TFn>
        auto FoldRight(TInit&& Init, TFn Fn)
        {
            return TFoldMixin::Fold(std::type_identity<typename TView::reference>{}, UR_FWD(Init), Fn, * static_cast<TView*>(this), Misc::Reverse);
        }

        template<typename TInit, typename TFn>
        auto FoldRight(TInit&& Init, TFn Fn) const
        {
            return TFoldMixin::Fold(std::type_identity<typename TView::const_reference>{}, UR_FWD(Init), Fn, * static_cast<const TView*>(this), Misc::Reverse);
        }

        template<typename TFn>
        auto FoldRight(TFn Fn)
        {
            return TFoldMixin::Fold(std::type_identity<typename TView::reference>{}, Fn, * static_cast<TView*>(this), Misc::Reverse);
        }

        template<typename TFn>
        auto FoldRight(TFn Fn) const
        {
            return TFoldMixin::Fold(std::type_identity<typename TView::const_reference>{}, Fn, * static_cast<const TView*>(this), Misc::Reverse);
        }
    };
}