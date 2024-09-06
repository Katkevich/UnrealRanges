#pragma once
#include "UnrealRanges/Detail/CursorApi.h"
#include <type_traits>
#include <concepts>

namespace Ur::View {

    template<typename TView>
    class TIterator
    {
    public:
        using value_type = typename TView::value_type;
        using reference = std::conditional_t<
            std::is_const_v<TView>,
            typename TView::const_reference,
            typename TView::reference
        >;
        using pointer = std::conditional_t<
            std::is_lvalue_reference_v<reference>,
            std::add_pointer_t<std::remove_reference_t<reference>>,
            void
        >;

        // some UE iterators aren't semiregular (TDoubleLinkedList iterator has no default ctor for instance)
        TIterator() requires std::semiregular<typename TView::Cursor> = default;

        template<typename UView>
        TIterator(typename UView::Cursor InCurs, const UView& InView)
            : Curs(InCurs)
            , View(std::addressof(InView))
        {
        }

        reference operator*() const
        {
            return FCursorApi::CursorDeref(*View, Curs);
        }

        TIterator<TView>& operator++()
        {
            FCursorApi::CursorInc(*View, Curs);
            return *this;
        }

        bool operator==(const TIterator<TView>& That) const
        {
            // default iterator is equivalent to end() iterator
            return View == That.View && (View == nullptr || FCursorApi::CursorEq(*View, Curs, That.Curs));
        }

        bool operator!=(const TIterator<TView>& That) const
        {
            return !operator==(That);
        }

    private:
        typename TView::Cursor Curs{};
        const TView* View{};
    };

    template<typename TView>
    struct TIteratorMixin
    {
        using iterator = TIterator<TView>;
        using const_iterator = TIterator<const TView>;

        auto begin()
        {
            return TIterator<TView>{ FCursorApi::CursorBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto end()
        {
            return TIterator<TView>{ FCursorApi::CursorEnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto begin() const
        {
            return TIterator<const TView>{ FCursorApi::CursorBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto end() const
        {
            return TIterator<const TView>{ FCursorApi::CursorEnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cbegin()
        {
            return TIterator<const TView>{ FCursorApi::CursorBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cend()
        {
            return TIterator<const TView>{ FCursorApi::CursorEnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cbegin() const
        {
            return TIterator<const TView>{ FCursorApi::CursorBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cend() const
        {
            return TIterator<const TView>{ FCursorApi::CursorEnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }
    };
}