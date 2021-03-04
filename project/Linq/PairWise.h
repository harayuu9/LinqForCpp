#pragma once
#include "TypeTraits.h"
#include "IteratorBase.h"

namespace linq {

class PairWiseBuilder
{
    /**
     * \brief Impl have iterator
     * \tparam Array ArrayType
     */
    template<class Array>
    class Impl
    {
    public:
        explicit Impl( Array&& arr ) noexcept : mArray( std::move( arr ) )
        {
        }

        template<class RawItr>
        struct Iterator : IteratorBase<RawItr>
        {
            using value_type = std::pair<typename iterator_traits<RawItr>::value_type, typename iterator_traits<RawItr>::value_type>;
            using pointer = std::add_pointer_t<value_type>;
            using reference = std::add_lvalue_reference_t<value_type>;

            Iterator() = default;
            explicit Iterator( RawItr beginItr ) noexcept
            {
                IteratorBase<RawItr>::mRawIterator = std::move(beginItr);
            }

            auto operator->() const noexcept
            {
                auto workItr = IteratorBase<RawItr>::mRawIterator;
                ++workItr;
                return std::make_pair(*IteratorBase<RawItr>::mRawIterator, *workItr);
            }

            auto operator*() const noexcept
            {
                auto workItr = IteratorBase<RawItr>::mRawIterator;
                ++workItr;
                return std::make_pair( *IteratorBase<RawItr>::mRawIterator, *workItr);
            }
        };

        [[nodiscard]] auto begin() const noexcept
        {
            auto beginItr = Begin( mArray );
            return Iterator<decltype(beginItr)>( beginItr );
        }

        [[nodiscard]] auto end() const noexcept
        {
            auto endItr = End( mArray );
            if constexpr (is_bidi_iter_v<decltype(endItr)>)
            {
                return Iterator<decltype(endItr)>(endItr - 1);
            }
            else
            {
                auto beginItr = Begin(mArray);
                while(true)
                {
                    auto work = beginItr;
                    ++work;
                    if (work == endItr)
                        return Iterator<decltype(beginItr)>(beginItr);
                    ++beginItr;
                }
            }
        }

    private:
        Array mArray;
    };

public:

    template<class Array>
    auto build( Array&& arr ) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");

        if constexpr ( std::is_lvalue_reference_v<Array> )
        {
            return Impl<decltype(&arr)>( &arr );
        }
        else
        {
            return Impl<Array>( std::forward<Array>( arr ) );
        }
    }
};

inline PairWiseBuilder PairWise() noexcept
{
    return PairWiseBuilder();
}
}
