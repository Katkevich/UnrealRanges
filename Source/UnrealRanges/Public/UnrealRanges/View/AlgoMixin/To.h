#pragma once
#include "UnrealRanges/Detail/CursorProtocol.h"
#include "UnrealRanges/View/View.h"

namespace Ur::View {
    template<typename TView>
    struct TToMixin
    {
        template<template<typename...> typename TContainer>
        auto To()
        {
            TContainer<typename TView::value_type> Result;

            FCursorProtocol::InternalIteration(Direction::Forward, *static_cast<TView*>(this), [&](auto&& Item)
                {
                    Result.Add(Item);
                    return Misc::ELoop::Continue;
                });

            return Result;
        }
    };
}