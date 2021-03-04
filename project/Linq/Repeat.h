#pragma once
#include "TypeTraits.h"
#include <string_view>

namespace linq {
template<typename T>
class RepeatBuilder
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
        Iterator(const T& initValue, const std::size_t currentRange, const std::size_t maxRange) noexcept : mValue( initValue )
        {
            mCurrentRange = currentRange;
            mMaxRange = maxRange;
        }
        void swap(Iterator& itr1, Iterator& itr2)
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
            return mValue;
        }

        auto operator*() const noexcept
        {
            return mValue;
        }

        bool operator==(const Iterator& other) const noexcept
        {
            return mCurrentRange == other.mCurrentRange;
        }

        bool operator!=(const Iterator& other) const noexcept
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
        difference_type operator-(const Iterator& other) const noexcept
        {
            return mCurrentRange - other.mCurrentRange;
        }
        Iterator operator[](const difference_type idx)
        {
            return Iterator(mValue, mCurrentRange + idx, mMaxRange);
        }
    private:
        const T mValue;
        std::size_t mCurrentRange, mMaxRange;
    };
public:
    explicit RepeatBuilder(const T& value, const std::size_t cnt) noexcept : mValue(value), mCnt(cnt)
    {
    }

    [[nodiscard]] auto begin() const noexcept
    {
        return Iterator(mValue, 0, mCnt);
    }

    [[nodiscard]] auto end() const noexcept
    {
        return Iterator(mValue, mCnt, mCnt);
    }

private:
    const T mValue;
    std::size_t mCnt;
};

template<typename T>
auto Repeat(const T& value, const std::size_t cnt)
{
    // literal convert string_view
    if constexpr(std::is_convertible_v<T, std::string_view>)
    {
        return RepeatBuilder<std::string_view>(std::string_view(value), cnt);
    }
    else
    {
        return RepeatBuilder<T>(value, cnt);
    }
}

}
