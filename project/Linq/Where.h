#pragma once
#include "TypeTraits.h"
#include "IteratorBase.h"

#define WHERE(variant, cond) ::linq::Where([&](const auto& (variant)){ return cond; })
namespace linq {

template<typename Func>
class WhereBuilder
{
    /**
     * \brief Can call Where
     *         Function result must be bool
     * \tparam T Have iterator
     */
    template<class T>
    class is_called_where
    {
        template<class U> static auto test( int ) -> decltype(std::declval<Func>()( *std::begin( std::declval<U>() ) ));
        template<class U> static void test( ... );
        template<class U> static auto constTest( int ) -> decltype(std::declval<Func>()( *std::begin( std::declval<const U>() ) ));
        template<class U> static void constTest( ... );
    public:
        static constexpr bool value = std::is_same_v<decltype(test<T>( 0 )), bool> || std::is_same_v<decltype(constTest<T>( 0 )), bool>;
    };

    /**
     * \brief Impl have iterator
     * \tparam Array ArrayType
     */
    template<class Array>
    class Impl
    {
    public:
        Impl( Impl&& other ) = default;
        Impl& operator=( Impl&& other ) = default;

        Impl( Array&& arr, Func&& func ) noexcept : mArray( std::move( arr ) ), mFunc( std::move( func ) )
        {
        }

        template<class RawItr>
        struct Iterator : IteratorBase<RawItr>
        {
            using iterator_category = std::forward_iterator_tag;

            Iterator() = default;
            explicit Iterator( RawItr beginItr, RawItr endItr, const Func* pFunc ) noexcept : mpFunc( pFunc ), mEndIterator( std::move( endItr ) )
            {
                IteratorBase<RawItr>::mRawIterator = std::move( beginItr );
                checkAndAdvanceItr_();
            }

            Iterator& operator++( int ) noexcept
            {
                ++IteratorBase<RawItr>::mRawIterator;
                checkAndAdvanceItr_();
                return *this;
            }

            Iterator& operator++() noexcept
            {
                ++IteratorBase<RawItr>::mRawIterator;
                checkAndAdvanceItr_();
                return *this;
            }
        private:
            void checkAndAdvanceItr_() noexcept
            {
                while ( IteratorBase<RawItr>::mRawIterator != mEndIterator && !( *mpFunc )( *IteratorBase<RawItr>::mRawIterator ) )
                {
                    ++IteratorBase<RawItr>::mRawIterator;
                }
            }
            const Func* mpFunc = nullptr;
            RawItr mEndIterator;
        };

        [[nodiscard]] auto begin() const noexcept
        {
            auto beginItr = Begin( mArray );
            auto endItr = End( mArray );
            return Iterator<decltype(beginItr)>( beginItr, endItr, &mFunc );
        }

        [[nodiscard]] auto end() const noexcept
        {
            auto endItr = End( mArray );
            return Iterator<decltype(endItr)>( endItr, endItr, &mFunc );
        }

    private:
        Array mArray;
        Func mFunc;
    };

public:
    explicit WhereBuilder( Func&& func ) noexcept : mFunc( std::move( func ) )
    {
    }

    template<class Array>
    auto build( Array&& arr ) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");
        static_assert(is_called_where<Array>::value, "function result is void or don't call function");

        if constexpr ( std::is_lvalue_reference_v<Array> )
        {
            return Impl<decltype(&arr)>( &arr, std::move( mFunc ) );
        }
        else
        {
            return Impl<Array>( std::forward<Array>( arr ), std::move( mFunc ) );
        }
    }

private:
    Func mFunc;
};

template<typename Func>
WhereBuilder<Func> Where( Func&& func ) noexcept
{
    return WhereBuilder<Func>( std::forward<Func>( func ) );
}
}
