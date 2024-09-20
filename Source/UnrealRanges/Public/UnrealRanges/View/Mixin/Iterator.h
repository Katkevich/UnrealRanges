#pragma once
#include "UnrealRanges/Detail/CursorProtocol.h"
#include <type_traits>
#include <concepts>

namespace Ur::View {

    template<typename TView, bool IsForward>
    class TIterator
    {
        using CursorType = TCursor<TView, IsForward>;

    public:
        using reference = TReference<TView>;
        using value_type = typename TView::value_type;
        using pointer = std::conditional_t<
            std::is_lvalue_reference_v<reference>,
            std::add_pointer_t<std::remove_reference_t<reference>>,
            void
        >;
        using difference_type = std::ptrdiff_t;

        // some UE iterators aren't semiregular (TDoubleLinkedList iterator has no default ctor for instance)
        TIterator() requires std::semiregular<CursorType> = default;

        TIterator(CursorType InCurs, TView& InView)
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
        TView* View{};
    };

    template<typename TView>
    struct TIteratorMixin
    {
        using iterator = TIterator<TView, Misc::Forward>;
        using const_iterator = TIterator<const TView, Misc::Forward>;

        auto begin()
        {
            return iterator{ FCursorProtocol::CursorBegin<Misc::Forward>(*static_cast<TView*>(this)), *static_cast<TView*>(this) };
        }

        auto end()
        {
            return iterator{ FCursorProtocol::CursorEnd<Misc::Forward>(*static_cast<TView*>(this)), *static_cast<TView*>(this) };
        }

        auto begin() const
        {
            return const_iterator{ FCursorProtocol::CursorBegin<Misc::Forward>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto end() const
        {
            return const_iterator{ FCursorProtocol::CursorEnd<Misc::Forward>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cbegin()
        {
            return const_iterator{ FCursorProtocol::CursorBegin<Misc::Forward>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cend()
        {
            return const_iterator{ FCursorProtocol::CursorEnd<Misc::Forward>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cbegin() const
        {
            return const_iterator{ FCursorProtocol::CursorBegin<Misc::Forward>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto cend() const
        {
            return const_iterator{ FCursorProtocol::CursorEnd<Misc::Forward>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }
    };

    template<typename TView>
    struct TReverseIteratorMixin
    {
        using reverse_iterator = TIterator<TView, Misc::Reverse>;
        using const_reverse_iterator = TIterator<const TView, Misc::Reverse>;

        auto rbegin()
        {
            return reverse_iterator{ FCursorProtocol::CursorBegin<Misc::Reverse>(*static_cast<TView*>(this)), *static_cast<TView*>(this) };
        }

        auto rend()
        {
            return reverse_iterator{ FCursorProtocol::CursorEnd<Misc::Reverse>(*static_cast<TView*>(this)), *static_cast<TView*>(this) };
        }

        auto rbegin() const
        {
            return const_reverse_iterator{ FCursorProtocol::CursorBegin<Misc::Reverse>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto rend() const
        {
            return const_reverse_iterator{ FCursorProtocol::CursorEnd<Misc::Reverse>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto crbegin()
        {
            return const_reverse_iterator{ FCursorProtocol::CursorBegin<Misc::Reverse>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto crend()
        {
            return const_reverse_iterator{ FCursorProtocol::CursorEnd<Misc::Reverse>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto crbegin() const
        {
            return const_reverse_iterator{ FCursorProtocol::CursorBegin<Misc::Reverse>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }

        auto crend() const
        {
            return const_reverse_iterator{ FCursorProtocol::CursorEnd<Misc::Reverse>(*static_cast<const TView*>(this)), *static_cast<const TView*>(this) };
        }
    };
}