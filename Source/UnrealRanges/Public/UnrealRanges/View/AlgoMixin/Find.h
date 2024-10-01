#pragma once
#include "UnrealRanges/View/Cursor.h"
#include "UnrealRanges/Optional.h"
#include "UnrealRanges/Traits.h"
#include "Templates/UnrealTemplate.h"
#include "Templates/IdentityFunctor.h"
#include "Algo/Find.h"

namespace Ur::View {

    namespace Detail
    {
        template<typename TView, bool IsForward>
        struct TFindMixin
        {
        protected:
            template<typename TRef, typename TValue, typename TSelf, typename TProj>
            static auto FindImpl(std::type_identity<TRef>, TValue&& Value, TSelf Self, TProj& Proj)
            {
                static_assert(EqComparableByWith<TRef, TProj, TValue&&>, "value (or projected value if projection is used) should be comparable with value using operator==");

                TOptional<TRef> Found{};
                Ur::Cursor::Iterate<IsForward>(*Self, [&](auto&& Item)
                    {
                        if (std::invoke(Proj, Item) == Value)
                        {
                            Found = UR_FWD(Item);

                            return Ur::ELoop::Break;
                        }
                        return Ur::ELoop::Continue;
                    });

                return Found;
            }

            template<typename TRef, typename TValue, typename TSelf, typename TProj>
            static bool ContainsImpl(std::type_identity<TRef>, TValue&& Value, TSelf Self, TProj& Proj)
            {
                static_assert(EqComparableByWith<TRef, TProj, TValue&&>, "value (or projected value if projection is used) should be comparable with value using operator==");

                bool Found = false;
                Ur::Cursor::Iterate<IsForward>(*Self, [&](auto&& Item)
                    {
                        if (std::invoke(Proj, UR_FWD(Item)) == Value)
                        {
                            Found = true;

                            return Ur::ELoop::Break;
                        }
                        return Ur::ELoop::Continue;
                    });

                return Found;
            }

            template<typename TRef, typename TSelf, typename TPred>
            static auto FindByImpl(std::type_identity<TRef>, TSelf Self, TPred& Pred)
            {
                static_assert(std::is_invocable_r_v<bool, TPred, TRef>, "TProj hould be invokable over range items");

                TOptional<TRef> Found{};
                Ur::Cursor::Iterate<IsForward>(*Self, [&](auto&& Item)
                    {
                        if (std::invoke(Pred, Item))
                        {
                            Found = UR_FWD(Item);

                            return Ur::ELoop::Break;
                        }
                        return Ur::ELoop::Continue;
                    });

                return Found;
            }

            template<typename TRef, typename TSelf, typename TPred>
            static bool ContainsByImpl(std::type_identity<TRef>, TSelf Self, TPred& Pred)
            {
                static_assert(std::is_invocable_r_v<bool, TPred, TRef>, "TProj hould be invokable over range items");

                bool Found = false;
                Ur::Cursor::Iterate<IsForward>(*Self, [&](auto&& Item)
                    {
                        if (std::invoke(Pred, UR_FWD(Item)))
                        {
                            Found = true;

                            return Ur::ELoop::Break;
                        }
                        return Ur::ELoop::Continue;
                    });

                return Found;
            }

            template<typename TRef, typename TValuesRng, typename TSelf, typename TProj>
            static auto FindFirstOfImpl(std::type_identity<TRef>, TValuesRng&& Values, TSelf Self, TProj& Proj)
            {
                static_assert(EqComparableByWith<TRef, TProj, RangeValue<TValuesRng>>, "values (or projected value if projection is used) should be comparable with value using operator==");

                TOptional<TRef> Found{};
                Ur::Cursor::Iterate<IsForward>(*Self, [&](auto&& Item)
                    {
                        if (Algo::Find(Values, std::invoke(Proj, Item)))
                        {
                            Found = UR_FWD(Item);

                            return Ur::ELoop::Break;
                        }
                        return Ur::ELoop::Continue;
                    });

                return Found;
            }
        };
    }

    template<typename TView>
    struct TFindFirstMixin : public Detail::TFindMixin<TView, Misc::Forward>
    {
    private:
        using TFindMixin = Detail::TFindMixin<TView, Misc::Forward>;

    public:
        template<typename TValue, typename TProj = FIdentityFunctor>
        auto Find(TValue&& Value, TProj Proj = {})
        {
            return TFindMixin::FindImpl(std::type_identity<typename TView::reference>{}, UR_FWD(Value), static_cast<TView*>(this), Proj);
        }

        template<typename TValue, typename TProj = FIdentityFunctor>
        auto Find(TValue&& Value, TProj Proj = {}) const
        {
            return TFindMixin::FindImpl(std::type_identity<typename TView::const_reference>{}, UR_FWD(Value), static_cast<const TView*>(this), Proj);
        }

        template<typename TValue, typename TProj = FIdentityFunctor>
        bool Contains(TValue&& Value, TProj Proj = {})
        {
            return TFindMixin::ContainsImpl(std::type_identity<typename TView::reference>{}, UR_FWD(Value), static_cast<TView*>(this), Proj);
        }

        template<typename TValue, typename TProj = FIdentityFunctor>
        bool Contains(TValue&& Value, TProj Proj = {}) const
        {
            return TFindMixin::ContainsImpl(std::type_identity<typename TView::const_reference>{}, UR_FWD(Value), static_cast<const TView*>(this), Proj);
        }

        template<typename TPred>
        auto FindBy(TPred Pred)
        {
            return TFindMixin::FindByImpl(std::type_identity<typename TView::reference>{}, static_cast<TView*>(this), Pred);
        }

        template<typename TPred>
        auto FindBy(TPred Pred) const
        {
            return TFindMixin::FindByImpl(std::type_identity<typename TView::const_reference>{}, static_cast<const TView*>(this), Pred);
        }

        template<typename TPred>
        auto ContainsBy(TPred Pred)
        {
            return TFindMixin::ContainsByImpl(std::type_identity<typename TView::reference>{}, static_cast<TView*>(this), Pred);
        }

        template<typename TPred>
        auto ContainsBy(TPred Pred) const
        {
            return TFindMixin::ContainsByImpl(std::type_identity<typename TView::const_reference>{}, static_cast<const TView*>(this), Pred);
        }

        template<typename TValuesRng, typename TProj = FIdentityFunctor>
        auto FindFirstOf(TValuesRng&& Values, TProj Proj = {})
        {
            return TFindMixin::FindFirstOfImpl(std::type_identity<typename TView::reference>{}, UR_FWD(Values), static_cast<TView*>(this), Proj);
        }

        template<typename TValuesRng, typename TProj = FIdentityFunctor>
        auto FindFirstOf(TValuesRng&& Values, TProj Proj = {}) const
        {
            return TFindMixin::FindFirstOfImpl(std::type_identity<typename TView::const_reference>{}, UR_FWD(Values), static_cast<const TView*>(this), Proj);
        }
    };

    template<typename TView>
    struct TFindLastMixin : public Detail::TFindMixin<TView, Misc::Reverse>
    {
    private:
        using TFindMixin = Detail::TFindMixin<TView, Misc::Reverse>;

    public:
        template<typename TValue, typename TProj = FIdentityFunctor>
        auto FindLast(TValue&& Value, TProj Proj = {})
        {
            return TFindMixin::FindImpl(std::type_identity<typename TView::reference>{}, UR_FWD(Value), static_cast<TView*>(this), Proj);
        }

        template<typename TValue, typename TProj = FIdentityFunctor>
        auto FindLast(TValue&& Value, TProj Proj = {}) const
        {
            return TFindMixin::FindImpl(std::type_identity<typename TView::const_reference>{}, UR_FWD(Value), static_cast<const TView*>(this), Proj);
        }

        template<typename TPred>
        auto FindLastBy(TPred Pred)
        {
            return TFindMixin::FindByImpl(std::type_identity<typename TView::reference>{}, static_cast<TView*>(this), Pred);
        }

        template<typename TPred>
        auto FindLastBy(TPred Pred) const
        {
            return TFindMixin::FindByImpl(std::type_identity<typename TView::const_reference>{}, static_cast<const TView*>(this), Pred);
        }

        template<typename TValuesRng, typename TProj = FIdentityFunctor>
        auto FindLastOf(TValuesRng&& Values, TProj Proj = {})
        {
            return TFindMixin::FindFirstOfImpl(std::type_identity<typename TView::reference>{}, UR_FWD(Values), static_cast<TView*>(this), Proj);
        }

        template<typename TValuesRng, typename TProj = FIdentityFunctor>
        auto FindLastOf(TValuesRng&& Values, TProj Proj = {}) const
        {
            return TFindMixin::FindFirstOfImpl(std::type_identity<typename TView::const_reference>{}, UR_FWD(Values), static_cast<const TView*>(this), Proj);
        }
    };
}