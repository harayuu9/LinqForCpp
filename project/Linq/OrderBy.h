// ----------------------------------------------------------------------------------------------
// Copyright (c) 2021 Yuto Harada
// ----------------------------------------------------------------------------------------------
// This software is released under the MIT License, see LICENSE.
//
#pragma once
#include <algorithm>
#include <vector>

#define ORDER_BY(variant, cond, isAscending) ::linq::OrderBy([&](const auto& (variant)){ return cond;}, isAscending)
#define ORDER_BY_ASCENDING(variant, cond) ::linq::OrderByAscending([&](const auto& (variant)){ return cond;})
#define ORDER_BY_DESCENDING(variant, cond) ::linq::OrderByDescending([&](const auto& (variant)){ return cond;})
namespace linq {

template<typename Func>
class OrderByBuilder
{
    template<class Array>
    class ImplPtr
    {
        using RawConstItr = decltype(Begin( std::declval<Array>() ));
        using RawItr = std::remove_const_t<RawConstItr>;
        using ArrayType = std::remove_const_t<std::remove_reference_t<decltype(*std::declval<RawConstItr>())>>;
    public:
        ImplPtr( ImplPtr&& other ) = default;
        ImplPtr& operator=( ImplPtr&& other ) = default;

        ImplPtr( Array&& arr, Func&& func, const bool isAscending ) : mArray( std::move( arr ) ), mFunc( std::move( func ) ), mIsAscending( isAscending )
        {
            sort_();
        }

        [[nodiscard]] auto begin() const noexcept
        {
            return mIndexArray.begin();
        }

        auto begin() noexcept
        {
            return mIndexArray.begin();
        }

        [[nodiscard]] auto end() const noexcept
        {
            return mIndexArray.end();
        }

        auto end() noexcept
        {
            return mIndexArray.end();
        }

        /**
         * \brief For is_select_many(SFINAE).
         */
        static void orderBy();
        friend struct ToVectorBuilder;
        template<typename F>
        friend class ThenByBuilder;
    private:
        void sort_()
        {
            auto beginItr = Begin( mArray );
            auto endItr = End( mArray );
            const auto arraySize = static_cast<std::size_t>( std::distance( beginItr, endItr ) );
            mIndexArray.resize( arraySize );
            auto i = 0;
            for ( auto itr = beginItr; itr != endItr; ++itr )
            {
                mIndexArray[i] = *itr;
                i++;
            }

            auto ascending = [&]( const auto& itr1, const auto& itr2 )
            {
                return mFunc( itr1 ) < mFunc( itr2 );
            };
            auto descending = [&]( const auto& itr1, const auto& itr2 )
            {
                return mFunc( itr1 ) > mFunc( itr2 );
            };
            if ( mIsAscending )
                std::sort( mIndexArray.begin(), mIndexArray.end(), ascending );
            else
                std::sort( mIndexArray.begin(), mIndexArray.end(), descending );
        }

        Array mArray;
        Func mFunc;
        std::vector<ArrayType, Allocator<ArrayType>> mIndexArray;
        bool mIsAscending;
    };

    template<class Array>
    class ImplMove
    {
        using RawItr = decltype(std::begin( std::declval<Array>() ));
        using ArrayType = array_type_t<Array>;
    public:
        ImplMove( ImplMove&& other ) = default;
        ImplMove& operator=( ImplMove&& other ) = default;

        ImplMove( Array&& arr, Func&& func, const bool isAscending ) : mArray( std::move( arr ) ), mFunc( std::move( func ) ), mIsAscending( isAscending )
        {
            sort_();
        }

        [[nodiscard]] auto begin() const noexcept
        {
            return Begin( mArray );
        }

        auto begin() noexcept
        {
            return Begin( mArray );
        }

        [[nodiscard]] auto end() const noexcept
        {
            return End( mArray );
        }

        auto end() noexcept
        {
            return End( mArray );
        }

        /**
         * \brief For is_select_many(SFINAE).
         */
        static void orderMove();
        friend struct ToVectorBuilder;
        template<typename F>
        friend class ThenByBuilder;
    private:
        void sort_()
        {
            auto ascending = [&]( const auto& itr1, const auto& itr2 )
            {
                return mFunc( itr1 ) < mFunc( itr2 );
            };
            auto descending = [&]( const auto& itr1, const auto& itr2 )
            {
                return mFunc( itr1 ) > mFunc( itr2 );
            };

            if ( mIsAscending )
                std::sort( mArray.begin(), mArray.end(), descending );
            else
                std::sort( mArray.begin(), mArray.end(), ascending );
        }

        Array mArray;
        Func mFunc;
        bool mIsAscending;
    };

public:
    OrderByBuilder( Func&& func, const bool isAscending ) noexcept : mFunc( std::move( func ) ), mIsAscending( isAscending )
    {
    }

    template<class Array>
    auto build( Array&& arr ) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");

        if constexpr ( std::is_lvalue_reference_v<Array> )
        {
            return ImplPtr<decltype(&arr)>( &arr, std::move( mFunc ), mIsAscending );
        }
        else if constexpr ( can_sort_array_v<Array> )
        {
            return ImplMove<Array>( std::forward<Array>( arr ), std::move( mFunc ), mIsAscending );
        }
        else
        {
            return ImplPtr<Array>( std::forward<Array>( arr ), std::move( mFunc ), mIsAscending );
        }
    }

private:
    Func mFunc;
    bool mIsAscending;
};

template<class Array>
class get_order_by_type
{
    template<typename U> static auto test( int ) -> decltype(U::orderBy(), std::integral_constant<int, 0>());
    template<typename U> static auto test( int ) -> decltype(U::orderMove(), std::integral_constant<int, 1>());
    template<typename U> static auto test( ... ) -> std::integral_constant<int, -1>;
public:
    using Type = decltype(test<Array>( 1 ));
};

template<class Array>
constexpr int get_order_by_type_v = get_order_by_type<std::remove_reference_t<Array>>::Type::value;

template<typename Func>
OrderByBuilder<Func> OrderBy( Func&& func, const bool isAscending = true )
{
    return OrderByBuilder<Func>( std::forward<Func>( func ), isAscending );
}

template<typename Func>
OrderByBuilder<Func> OrderByAscending( Func&& func )
{
    return OrderByBuilder<Func>( std::forward<Func>( func ), true );
}

template<typename Func>
OrderByBuilder<Func> OrderByDescending( Func&& func )
{
    return OrderByBuilder<Func>( std::forward<Func>( func ), false );
}
}
