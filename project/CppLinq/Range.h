#pragma once
#include "TypeTraits.h"

namespace linq {

template<typename T>
class RangeBuilder
{
    struct Iterator
    {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::size_t;
        using pointer = T*;
        using reference = T&;

        Iterator() = default;
        ~Iterator() = default;
        Iterator(const Iterator&) = default;
        Iterator(const T initValue, const T currentRange, const T maxRange) noexcept
        {
            mInitValue = initValue;
            mCurrentRange = currentRange;
            mMaxRange = maxRange;
        }
        void swap(Iterator & itr1, Iterator & itr2)
        {
            std::swap(itr1, itr2);
        }

        Iterator& operator++() noexcept
        {
            ++mCurrentRange;
            return *this;
        }
        Iterator operator++(int) noexcept
        {
            ++mCurrentRange;
            return *this;
        }

        auto operator->() const noexcept
        {
            return mInitValue + mCurrentRange;
        }

        auto operator*() const noexcept
        {
            return mInitValue + mCurrentRange;
        }

        bool operator==(const Iterator & other) const noexcept
        {
            return mCurrentRange == other.mCurrentRange;
        }

        bool operator!=(const Iterator & other) const noexcept
        {
            return !(*this == other);
        }

        Iterator& operator--()
        {
            --mCurrentRange;
            return *this;
        }
        Iterator operator--(int)
        {
            --mCurrentRange;
            return *this;
        }
        Iterator operator+(const difference_type idx)
        {
            return mCurrentRange + idx;
        }
        Iterator& operator+=(const difference_type idx)
        {
            return mCurrentRange += idx;
        }
        Iterator operator-(const difference_type idx)
        {
            return mCurrentRange - idx;
        }
        Iterator operator-=(const difference_type idx)
        {
            return mCurrentRange -= idx;
        }
        difference_type operator-(const Iterator & other) const noexcept
        {
            return mCurrentRange - other.mCurrentRange;
        }
        Iterator operator[](const difference_type idx)
        {
            return Iterator(mInitValue ,mCurrentRange + idx, mMaxRange);
        }
    private:
        T mInitValue;
        T mCurrentRange, mMaxRange;
    };
public:
    explicit RangeBuilder(const T value, const T cnt) noexcept : mInit(value), mCnt( cnt )
    {
    }

    [[nodiscard]] auto begin() const noexcept
    {
        return Iterator(mInit, 0, mCnt);
    }

    [[nodiscard]] auto end() const noexcept
    {
        return Iterator(mInit, mCnt, mCnt);
    }

private:
    T mInit;
    T mCnt;
};

template<typename T>
auto Range(const T value, const T cnt)
{
    return RangeBuilder<T>(value, cnt);
}

}
