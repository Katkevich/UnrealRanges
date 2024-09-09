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

    template<typename TView>
    struct TToMixin
    {
        template<template<typename...> typename TContainer>
        auto To()
        {
            using ElementType = typename TView::value_type;

            if constexpr (TIsMapTemplate_V<TContainer>)
            {
                using KeyType = decltype(std::declval<ElementType>().Key);
                using ValueType = decltype(std::declval<ElementType>().Value);

                TContainer<KeyType, ValueType> Target;
                this->Into(Target);

                return Target;
            }
            else if constexpr (TIsSameTemplate_V<TContainer, TIndirectArray> && (TIsTUniquePtr_V<ElementType> || TIsOwned_V<ElementType>))
            {
                using UnderlyingType = ElementType::ElementType;

                TIndirectArray<UnderlyingType> Target;
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

            // MSVC has a bug which forbids the usage of "requires" expression inside lambda...
            constexpr bool IsIndirectArray = TIsIndirectArray_V<TContainer>;
            constexpr bool IsStaticArray = TIsStaticArray_V<TContainer>;
            constexpr bool HasAddElement = requires{ Target.AddElement(std::declval<ElementType>()); };
            constexpr bool HasAdd = requires{ Target.Add(std::declval<ElementType>()); };
            constexpr bool HasAddTail = requires{ Target.AddTail(std::declval<ElementType>()); };
            constexpr bool HasPushLast = requires{ Target.PushLast(std::declval<ElementType>()); };
            constexpr bool HasAppendInt = requires{ Target.AppendInt(std::declval<ElementType>()); };
            constexpr bool HasAppendChar = requires{ Target.AppendChar(std::declval<ElementType>()); };
            constexpr bool HasAppend = requires{ Target.Append(std::declval<ElementType>()); };

            static_assert(IsIndirectArray || IsStaticArray || HasAdd || HasAddTail || HasPushLast || HasAppendInt || HasAppendChar || HasAppend,
                "the target container does not support adding elements in the intended way");

            int32 Index = 0;

            FCursorProtocol::InternalIteration(Misc::Forward, *static_cast<TView*>(this), [&](auto&& Item)
                {
                    // TIndirectArray
                    if constexpr (IsIndirectArray)
                    {
                        if constexpr (TIsTUniquePtr_V<ElementType>)
                        {
                            Target.Add(Item.Release());
                        }
                        else if constexpr (TIsOwned_V<ElementType>)
                        {
                            Target.Add(Item.Ptr);
                        }
                        else
                        {
                            static_assert(Detail::TAlwaysFalse<TContainer>::Value, "sequence of [TUniquePtr<T>] or [TOwned<T>] are only supported for now as a source for TIndirectArray");
                        }
                    }
                    // TStaticArray
                    else if constexpr (IsStaticArray)
                    {
                        // make sure we do not exceed array size
                        if (Index < Target.Num())
                            Target[Index++] = UR_FWD(Item);
                        else
                            return Misc::ELoop::Break;
                    }
                    // TChunkedArray (Add method has different meaning for TChunkedArray)
                    else if constexpr (HasAddElement)
                    {
                        Target.AddElement(UR_FWD(Item));
                    }
                    // TArray, TSet, TSparseArray
                    else if constexpr (HasAdd)
                    {
                        Target.Add(UR_FWD(Item));
                    }
                    // TDoubleLinkedList
                    else if constexpr (HasAddTail)
                    {
                        Target.AddTail(UR_FWD(Item));
                    }
                    // TDeque
                    else if constexpr (HasPushLast)
                    {
                        Target.PushLast(UR_FWD(Item));
                    }
                    // FString
                    // checking the exact type so that we do not hide AppendInt
                    else if constexpr (HasAppendChar && std::is_same_v<TCHAR, std::remove_cvref_t<ElementType>>)
                    {
                        Target.AppendChar(UR_FWD(Item));
                    }
                    // FString
                    else if constexpr (HasAppendInt)
                    {
                        Target.AppendInt(UR_FWD(Item));
                    }
                    // FString
                    else if constexpr (HasAppend)
                    {
                        Target.Append(UR_FWD(Item));
                    }

                    return Misc::ELoop::Continue;
                });
        }
    };
}