#pragma once
#include "TypeTraits.h"
#include "IteratorBase.h"

namespace linq {
template<class ZipArray>
class ZipWithBuilder
{
    using ZipArrayIterator = decltype(Begin( std::declval<ZipArray>() ));

    template<class Array>
    class Impl
    {
    public:
        Impl( Impl&& other ) = default;
        Impl& operator=( Impl&& other ) = default;

        Impl( Array&& arr, ZipArray&& zipArr ) noexcept : mArray( std::move( arr ) ), mZipArray( std::move( zipArr ) )
        {
        }

        template<class RawItr>
        struct Iterator : IteratorBase<RawItr>
        {
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<array_type_t<Array>, array_type_t<ZipArray>>;
            using pointer = std::add_pointer_t<value_type>;
            using reference = std::add_lvalue_reference_t<value_type>;

            Iterator() = default;

            Iterator( RawItr itr, ZipArrayIterator zipItr ) noexcept
            {
                IteratorBase<RawItr>::mRawIterator = std::move( itr );
                mZipIterator = std::move( zipItr );
            }

            Iterator& operator++() noexcept
            {
                ++IteratorBase<RawItr>::mRawIterator;
                ++mZipIterator;
                return *this;
            }

            Iterator operator++( int ) noexcept
            {
                ++IteratorBase<RawItr>::mRawIterator;
                ++mZipIterator;
                return *this;
            }

            auto operator->() const noexcept
            {
                return std::make_pair( *IteratorBase<RawItr>::mRawIterator, *mZipIterator );
            }

            auto operator*() const noexcept
            {
                return std::make_pair( *IteratorBase<RawItr>::mRawIterator, *mZipIterator );
            }

        private:
            ZipArrayIterator mZipIterator;
        };

        [[nodiscard]] auto begin() const noexcept
        {
            auto rawBeginItr = Begin( mArray );
            auto zipBeginItr = Begin( mZipArray );
            return Iterator<decltype(rawBeginItr)>( rawBeginItr, zipBeginItr );
        }

        [[nodiscard]] auto end() const noexcept
        {
            auto rawBeginItr = Begin( mArray );
            auto zipBeginItr = Begin( mZipArray );
            const auto rawEndItr = End( mArray );
            const auto zipEndItr = End( mZipArray );
            const auto rawCnt = static_cast<std::size_t>( std::distance( rawBeginItr, rawEndItr ) );
            const auto zipCnt = static_cast<std::size_t>( std::distance( zipBeginItr, zipEndItr ) );
            const auto min = std::min( rawCnt, zipCnt );
            std::advance( rawBeginItr, min );
            std::advance( zipBeginItr, min );
            return Iterator<decltype(rawBeginItr)>( rawBeginItr, zipBeginItr );
        }

    private:
        Array mArray;
        ZipArray mZipArray;
    };

public:
    explicit ZipWithBuilder( ZipArray&& arr ) noexcept : mZipArray( std::move( arr ) )
    {
    }

    template<class Array>
    auto build( Array&& arr ) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");

        if constexpr ( std::is_lvalue_reference_v<Array> )
        {
            return Impl<decltype(&arr)>( &arr, std::move( mZipArray ) );
        }
        else
        {
            return Impl<Array>( std::forward<Array>( arr ), std::move( mZipArray ) );
        }
    }

private:
    ZipArray mZipArray;
};

template<class ZipArray>
auto ZipWith( ZipArray&& arr ) noexcept
{
    static_assert(have_const_iterator_v<ZipArray>, "T have not iterator");
    if constexpr ( std::is_lvalue_reference_v<ZipArray> )
    {
        return ZipWithBuilder<decltype(&arr)>( &arr );
    }
    else
    {
        return ZipWithBuilder<ZipArray>( std::forward<ZipArray>( arr ) );
    }
}

}
