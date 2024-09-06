#pragma once
#include "UnrealRanges/Detail/CursorProtocol.h"
#include <type_traits>
#include <concepts>

namespace Ur::View {

    template<typename TView, bool Reverse>
    class TIterator
    {
        using CursorType = std::conditional_t<Reverse, typename TView::Cursor, typename TView::ReverseCursor>;

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
        TIterator() requires std::semiregular<CursorType> = default;

        template<typename UView, typename UCursor>
        TIterator(UCursor InCurs, const UView& InView)
            : Curs(InCurs)
            , View(std::addressof(InView))
        {
        }

        reference operator*() const
        {
            return FCursorProtocol::CursorDeref(*View, Curs);
        }

        TIterator& operator++()
        {
            FCursorProtocol::CursorInc(*View, Curs);
            return *this;
        }

        bool operator==(const TIterator& That) const
        {
            // default iterator is equivalent to end() iterator
            return View == That.View && (View == nullptr || FCursorProtocol::CursorEq(*View, Curs, That.Curs));
        }

        bool operator!=(const TIterator& That) const
        {
            return !operator==(That);
        }

    private:
        CursorType Curs{};
        const TView* View{};
    };

    template<typename TView>
    struct TIteratorMixin
    {
        using iterator = TIterator<TView, true>;
        using const_iterator = TIterator<const TView, true>;

        auto begin()
        {
            return iterator{ FCursorProtocol::CursorBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto end()
        {
            return iterator{ FCursorProtocol::CursorEnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto begin() const
        {
            return const_iterator{ FCursorProtocol::CursorBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto end() const
        {
            return const_iterator{ FCursorProtocol::CursorEnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cbegin()
        {
            return const_iterator{ FCursorProtocol::CursorBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cend()
        {
            return const_iterator{ FCursorProtocol::CursorEnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cbegin() const
        {
            return const_iterator{ FCursorProtocol::CursorBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cend() const
        {
            return const_iterator{ FCursorProtocol::CursorEnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }
    };

    template<typename TView>
    struct TReverseIteratorMixin
    {
        using reverse_iterator = TIterator<TView, false>;
        using const_reverse_iterator = TIterator<const TView, false>;

        auto rbegin()
        {
            return reverse_iterator{ FCursorProtocol::CursorRBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto rend()
        {
            return reverse_iterator{ FCursorProtocol::CursorREnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto rbegin() const
        {
            return const_reverse_iterator{ FCursorProtocol::CursorRBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto rend() const
        {
            return const_reverse_iterator{ FCursorProtocol::CursorREnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto crbegin()
        {
            return const_reverse_iterator{ FCursorProtocol::CursorRBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto crend()
        {
            return const_reverse_iterator{ FCursorProtocol::CursorREnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto crbegin() const
        {
            return const_reverse_iterator{ FCursorProtocol::CursorRBegin(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto crend() const
        {
            return const_reverse_iterator{ FCursorProtocol::CursorREnd(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }
    };
}