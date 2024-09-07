#pragma once
#include "UnrealRanges/Detail/CursorProtocol.h"
#include "UnrealRanges/Detail/AlwaysFalse.h"
#include "UnrealRanges/UnrealTraits.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Owned.h"
#include "Containers/IndirectArray.h"
#include "Templates/UniquePtr.h"
#include <type_traits>

namespace Ur::View {

    template<typename T>
    struct foo;

    template<typename T>
    concept OwnedPtr = TIsTUniquePtr_V<T> || TIsOwned_V<T>;

    template<typename TView>
    struct TToMixin
    {
        template<template<typename...> typename TContainer>
        auto To()
        {
            using ElementType = typename TView::value_type;

            if constexpr (Ur::TIsMapTemplate_V<TContainer>)
            {
                using KeyType = decltype(std::declval<ElementType>().Key);
                using ValueType = decltype(std::declval<ElementType>().Value);

                TContainer<KeyType, ValueType> Target;
                this->Into(Target);

                return Target;
            }
            else
            {
                TContainer<ElementType> Target;
                this->Into(Target);
                return Target;
            }
        }

        template<>
        auto To<TIndirectArray>() requires OwnedPtr<typename TView::value_type>
        {
            using ElementType = typename TView::value_type;
            using UnderlyingType = ElementType::ElementType;

            TIndirectArray<UnderlyingType> Target;

            this->Into(Target);

            return Target;
        }

        template<typename TContainer>
        auto ToExactly()
        {
            TContainer Result;
            this->Into(Result);

            return Result;
        }

        template<typename TContainer>
        void Into(TContainer& Target)
        {
            using ElementType = typename TView::value_type;

            if constexpr (TIsIndirectArray_V<TContainer>)
            {
                if constexpr (TIsTUniquePtr_V<ElementType>)
                {
                    this->IntoImpl(Target, &ElementType::Release);
                }
                else if constexpr (TIsOwned_V<ElementType>)
                {
                    this->IntoImpl(Target, &ElementType::Ptr);
                }
                else
                {
                    static_assert(Detail::TAlwaysFalse<TContainer>::Value, "sequence of [TUniquePtr<T>] or [TOwned<T>] are only supported for now as a source for TIndirectArray");
                }
            }
            else
            {
                this->IntoImpl(Target);
            }
        }

    private:
        template<typename TContainer, typename TProjection = FIdentityFunctor>
        void IntoImpl(TContainer& Target, TProjection Proj = {})
        {
            using ElementType = std::invoke_result_t<TProjection, typename TView::value_type>;

            // MSVC has a bug which forbids the usage of "requires" expression inside lambda...
            constexpr bool HasAddElement = requires{ Target.AddElement(std::declval<ElementType>()); };
            constexpr bool HasAdd = requires{ Target.Add(std::declval<ElementType>()); };
            constexpr bool HasAddTail = requires{ Target.AddTail(std::declval<ElementType>()); };
            constexpr bool HasPushLast = requires{ Target.PushLast(std::declval<ElementType>()); };
            constexpr bool HasAppendInt = requires{ Target.AppendInt(std::declval<ElementType>()); };
            constexpr bool HasAppendChar = requires{ Target.AppendChar(std::declval<ElementType>()); };
            constexpr bool HasAppend = requires{ Target.Append(std::declval<ElementType>()); };

            static_assert(HasAdd || HasAddTail || HasPushLast || HasAppendInt || HasAppendChar || HasAppend,
                "the target container does not support adding elements in the intended way");

            FCursorProtocol::InternalIteration(Direction::Forward, *static_cast<TView*>(this), [&](auto&& Item)
                {
                    // TChunkedArray (Add method has different meaning for TChunkedArray)
                    if constexpr (HasAddElement)
                    {
                        Target.AddElement(std::invoke(Proj, UR_FWD(Item)));
                    }
                    // TArray, TSet, TSparseArray
                    else if constexpr (HasAdd)
                    {
                        Target.Add(std::invoke(Proj, UR_FWD(Item)));
                    }
                    // TDoubleLinkedList
                    else if constexpr (HasAddTail)
                    {
                        Target.AddTail(std::invoke(Proj, UR_FWD(Item)));
                    }
                    // TDeque
                    else if constexpr (HasPushLast)
                    {
                        Target.PushLast(std::invoke(Proj, UR_FWD(Item)));
                    }
                    // FString
                    // checking the exact type so that we do not hide AppendInt
                    else if constexpr (HasAppendChar && std::is_same_v<TCHAR, std::remove_cvref_t<ElementType>>)
                    {
                        Target.AppendChar(std::invoke(Proj, UR_FWD(Item)));
                    }
                    // FString
                    else if constexpr (HasAppendInt)
                    {
                        Target.AppendInt(std::invoke(Proj, UR_FWD(Item)));
                    }
                    // FString
                    else if constexpr (HasAppend)
                    {
                        Target.Append(std::invoke(Proj, UR_FWD(Item)));
                    }

                    return Misc::ELoop::Continue;
                });
        }
    };
}