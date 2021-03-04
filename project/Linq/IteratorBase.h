// ----------------------------------------------------------------------------------------------
// Copyright (c) 2021 Yuto Harada
// ----------------------------------------------------------------------------------------------
// This software is released under the MIT License, see LICENSE.
//
#pragma once
#include "TypeTraits.h"

namespace linq {

template <class RawItr>
struct IteratorBase
{
    using iterator_category = typename iterator_traits<RawItr>::iterator_category;
    using value_type = typename iterator_traits<RawItr>::value_type;
    using difference_type = typename iterator_traits<RawItr>::difference_type;
    using pointer = typename iterator_traits<RawItr>::pointer;
    using reference = typename iterator_traits<RawItr>::reference;

    IteratorBase() = default;
    ~IteratorBase() = default;
    IteratorBase(const IteratorBase&) = default;
    void swap(IteratorBase& itr1, IteratorBase& itr2)
    {
        std::swap(itr1, itr2);
    }

    IteratorBase& operator++() noexcept
    {
        ++mRawIterator;
        return *this;
    }
    IteratorBase operator++(int) noexcept
    {
        ++mRawIterator;
        return *this;
    }

    pointer operator->() const noexcept
    {
        return mRawIterator;
    }

    reference operator*() const noexcept
    {
        return *mRawIterator;
    }

    bool operator==(const IteratorBase& other) const noexcept
    {
        return mRawIterator == other.mRawIterator;
    }

    bool operator!=(const IteratorBase& other) const noexcept
    {
        return !(*this == other);
    }

    template<bool U = is_bidi_iter_v<IteratorBase>, typename = std::enable_if_t<U>>
    IteratorBase& operator--() noexcept
    {
        --mRawIterator;
        return *this;
    }
    template<bool U = is_bidi_iter_v<IteratorBase>, typename = std::enable_if_t<U>>
    IteratorBase operator--(int) noexcept
    {
        --mRawIterator;
        return *this;
    }
    template<bool U = is_random_iter_v<IteratorBase>, typename = std::enable_if_t<U>>
    IteratorBase operator+(const difference_type idx) const noexcept
    {
        return mRawIterator + idx;
    }
    template<bool U = is_random_iter_v<IteratorBase>, typename = std::enable_if_t<U>>
    IteratorBase& operator+=(const difference_type idx) noexcept
    {
        return mRawIterator += idx;
    }
    template<bool U = is_random_iter_v<IteratorBase>, typename = std::enable_if_t<U>>
    IteratorBase operator-(const difference_type idx) const noexcept
    {
        return mRawIterator - idx;
    }
    template<bool U = is_random_iter_v<IteratorBase>, typename = std::enable_if_t<U>>
    IteratorBase operator-=(const difference_type idx) noexcept
    {
        return mRawIterator -= idx;
    }
    template<bool U = is_random_iter_v<IteratorBase>, typename = std::enable_if_t<U>>
    difference_type operator-(const IteratorBase& other) const noexcept
    {
        return mRawIterator - other.mRawIterator;
    }
    template<bool U = is_random_iter_v<IteratorBase>, typename = std::enable_if_t<U>>
    IteratorBase operator[](const difference_type idx) const noexcept
    {
        return mRawIterator[idx];
    }
protected:
    RawItr mRawIterator;
};

}
