// ----------------------------------------------------------------------------------------------
// Copyright (c) 2021 Yuto Harada
// ----------------------------------------------------------------------------------------------
// This software is released under the MIT License, see LICENSE.
//
#pragma once
#include "TypeTraits.h"

#define TAKE_WHILE(variant, cond) ::linq::TakeWhile([&](const auto& (variant)){ return cond; })
namespace linq {
class TakeBuilder
{
    template<class Array>
    class Impl
    {
    public:
        Impl( Impl&& other ) = default;
        Impl& operator=( Impl&& other ) = default;

        Impl( Array&& arr, const int cnt ) noexcept : mArray( std::move( arr ) ), mTakeCnt( cnt )
        {
        }

        [[nodiscard]] auto begin() const noexcept
        {
            return Begin( mArray );
        }

        [[nodiscard]] auto end() const noexcept
        {
            auto beginItr = Begin( mArray );
            const auto endItr = End( mArray );
            if constexpr ( is_random_iter_v<decltype(beginItr)> )
            {
                if ( endItr - beginItr < mTakeCnt )
                {
                    return endItr;
                }
                return beginItr + mTakeCnt;
            }
            else
            {
                auto cnt = 0;
                for ( auto itr = beginItr; itr != endItr; ++itr )
                {
                    cnt++;
                    if ( cnt > mTakeCnt )
                        return itr;
                }
                return endItr;
            }
        }

    private:
        Array mArray;
        int mTakeCnt;
    };

public:
    explicit TakeBuilder( const int cnt ) noexcept : mTakeCount( cnt )
    {
    }

    template<class Array>
    auto build( Array&& arr ) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");

        if constexpr ( std::is_lvalue_reference_v<Array> )
        {
            return Impl<decltype(&arr)>( &arr, mTakeCount );
        }
        else
        {
            return Impl<Array>( std::forward<Array>( arr ), mTakeCount );
        }
    }

private:
    int mTakeCount;
};

template<typename Func>
class TakeWhileBuilder
{
    template<class Array>
    class is_called_take_while
    {
        template<class U> static auto test( int ) -> decltype(std::declval<Func>()( *std::begin( std::declval<U>() ) ));
        template<class U> static void test( ... );
        template<class U> static auto constTest( int ) -> decltype(std::declval<Func>()( *std::begin( std::declval<const U>() ) ));
        template<class U> static void constTest( ... );
    public:
        static constexpr bool value = std::is_same_v<decltype(test<Array>( 0 )), bool> || std::is_same_v<decltype(constTest<Array>( 0 )), bool>;
    };

    template<class Array>
    class Impl
    {
    public:
        Impl( Impl&& other ) = default;
        Impl& operator=( Impl&& other ) = default;

        Impl( Array&& arr, Func&& func ) noexcept : mArray( std::move( arr ) ), mFunc( std::move( func ) )
        {
        }

        [[nodiscard]] auto begin() const noexcept
        {
            return Begin( mArray );
        }

        [[nodiscard]] auto end() const noexcept
        {
            auto beginItr = Begin( mArray );
            const auto endItr = End( mArray );
            auto cnt = 0;
            for ( auto itr = beginItr; itr != endItr; ++itr )
            {
                cnt++;
                if ( !mFunc( *itr ) )
                    return itr;
            }
            return endItr;
        }

    private:
        Array mArray;
        Func mFunc;
    };

public:
    explicit TakeWhileBuilder( Func&& func ) noexcept : mFunc( func )
    {
    }

    template<class Array>
    auto build( Array&& arr ) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");
        static_assert(is_called_take_while<Array>::value, "function result is void or don't call function");

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

inline TakeBuilder Take( const int cnt ) noexcept
{
    return TakeBuilder( cnt );
}

template<typename Func>
TakeWhileBuilder<Func> TakeWhile( Func&& func ) noexcept
{
    return TakeWhileBuilder<Func>( std::forward<Func>( func ) );
}
}
