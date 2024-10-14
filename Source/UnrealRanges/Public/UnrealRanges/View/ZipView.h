#pragma once
#include "UnrealRanges/View/RefView.h"
#include "UnrealRanges/Traits.h"
#include "UnrealRanges/Utility.h"
#include "Algo/MinElement.h"
#include <limits>
#include <array>
#include <type_traits>

namespace Ur::View {
    template<Ur::RangeView... TUnderlViews>
    class TZipView
        : public FView
        , public Detail::TMixins<TZipView<TUnderlViews...>, TDefaultMixins>
    {
        friend struct Ur::Cursor;

    public:
        using reference = TTuple<typename TUnderlViews::reference...>;
        using const_reference = TTuple<typename TUnderlViews::const_reference...>;
        using value_type = reference;

        struct Cursor
        {
            TTuple<typename TUnderlViews::Cursor...> Nested{};
        };
        struct ConstCursor
        {
            TTuple<typename TUnderlViews::ConstCursor...> Nested{};
        };
        using ReverseCursor = void;
        using ReverseConstCursor = void;

        static constexpr bool IsBidir = false;
        static constexpr bool IsSized = (TUnderlViews::IsSized && ...);
        static constexpr bool LikeMap = false;

        template<typename... UViews>
        TZipView(Misc::FFromViewTag, UViews&& ...InViews)
            : Views(UR_FWD(InViews)...)
        {
        }

        auto Num() const requires IsSized
        {
            using CommonSizeType = std::common_type_t<std::invoke_result_t<FSizeCpo, TUnderlViews>...>;

            const std::array Sizes = [&]<size_t... Indices>(std::index_sequence<Indices...>) {
                return std::array{ static_cast<CommonSizeType>(Ur::Size(get<Indices>(Views)))... };
            }(std::index_sequence_for<TUnderlViews...>{});

            return *Algo::MinElement(Sizes);
        }

    private:
        template<bool IsForward, typename TSelf, typename TCallback>
        UR_DEBUG_NOINLINE static Ur::ELoop InternalIterate(TSelf& Self, TCallback Callback)
        {
            // no implementation using only internal iteration (somehow we need to iterate multiple views simultaneously)
            // so fallback to default cursor-based iteration instead
            auto EndCurs = CursorEnd<IsForward>(Self);
            for (auto Curs = CursorBegin<IsForward>(Self); !CursorEq(Self, Curs, EndCurs); CursorInc(Self, Curs))
            {
                if (Callback(CursorDeref(Self, Curs)) == Ur::ELoop::Break)
                    break;
            }
            return Ur::ELoop::Break;
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorBegin(TSelf& Self)
        {
            return TCursor<TSelf, IsForward>
            {
                [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                    return MakeTuple(Ur::Cursor::Begin<IsForward>(get<Is>(Self.Views))...);
                }(std::index_sequence_for<TUnderlViews...>{})
            };
        }

        template<bool IsForward, typename TSelf>
        UR_DEBUG_NOINLINE static auto CursorEnd(TSelf& Self)
        {
            return TCursor<TSelf, IsForward>
            {
                [&] <std::size_t... Is>(std::index_sequence<Is...>) {
                    return MakeTuple(Ur::Cursor::End<IsForward>(get<Is>(Self.Views))...);
                }(std::index_sequence_for<TUnderlViews...>{})
            };
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static void CursorInc(TSelf& Self, TCursor& Curs)
        {
            [&] <std::size_t... Is>(std::index_sequence<Is...>) {
                (Ur::Cursor::Inc(get<Is>(Self.Views), get<Is>(Curs.Nested)), ...);
            }(std::index_sequence_for<TUnderlViews...>{});
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static decltype(auto) CursorDeref(TSelf& Self, const TCursor& Curs)
        {
            return [&]<std::size_t... Is>(std::index_sequence<Is...>) -> decltype(auto) {
                return TReference<TSelf>{ Ur::Cursor::Deref(get<Is>(Self.Views), get<Is>(Curs.Nested))... };
            }(std::index_sequence_for<TUnderlViews...>{});
        }

        template<typename TSelf, typename TCursor>
        UR_DEBUG_NOINLINE static bool CursorEq(TSelf& Self, const TCursor& Lhs, const TCursor& Rhs)
        {
            bool bIsAllEq = true;

            auto CmpCursors = [&](auto& View, auto& L, auto& R) {
                auto bIsEq = Ur::Cursor::Eq(View, L, R);
                bIsAllEq &= bIsEq;

                return bIsEq && Ur::Cursor::IsEnd(View, L); // if any of the cursors reached the end then the whole composite cursor itself reached the end
            };

            bool bHasCursorPairBothEnd = [&] <std::size_t... Is>(std::index_sequence<Is...>) {
                return (CmpCursors(get<Is>(Self.Views), get<Is>(Lhs.Nested), get<Is>(Rhs.Nested)) || ...);
            }(std::index_sequence_for<TUnderlViews...>{});

            if (bHasCursorPairBothEnd)
                return true;

            return bIsAllEq;
        }

    private:
        TTuple<TUnderlViews...> Views;
    };


    template<typename TRng, typename... TRngs>
    auto Zip(TRng& Rng, TRngs&... Rngs)
    {
        //TODO: own view & view in support
        return TZipView<TRefView<TRng>, TRefView<TRngs>...>({}, TRefView<TRng>({}, Rng), TRefView<TRngs>({}, Rngs)...);
    }
}