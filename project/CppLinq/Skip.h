#pragma once
#include "TypeTraits.h"

#define SKIP_WHILE(variant, cond) ::linq::SkipWhile([&](const auto& (variant)){ return cond; })
namespace linq {
class SkipBuilder
{
    template<class Array>
    class Impl
    {
    public:
        Impl( Impl&& other ) = default;
        Impl& operator=( Impl&& other ) = default;

        Impl( Array&& arr, const int cnt ) noexcept : mArray( std::move( arr ) ), mSkipCnt( cnt )
        {
        }

        [[nodiscard]] auto begin() const noexcept
        {
            auto beginItr = Begin( mArray );
            const auto endItr = End( mArray );
            if constexpr ( is_random_iter_v<decltype(beginItr)> )
            {
                if ( endItr - beginItr < mSkipCnt )
                {
                    return endItr;
                }
                return beginItr + mSkipCnt;
            }
            else
            {
                auto cnt = 0;
                for ( auto itr = beginItr; itr != endItr; ++itr )
                {
                    cnt++;
                    if ( cnt > mSkipCnt )
                        return itr;
                }
                return endItr;
            }
        }

        [[nodiscard]] auto end() const noexcept
        {
            return End( mArray );
        }

    private:
        Array mArray;
        int mSkipCnt;
    };

public:
    explicit SkipBuilder( const int cnt ) noexcept : mSkipCount( cnt )
    {
    }

    template<class Array>
    auto build( Array&& arr ) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");

        if constexpr ( std::is_lvalue_reference_v<Array> )
        {
            return Impl<decltype(&arr)>( &arr, mSkipCount );
        }
        else
        {
            return Impl<Array>( std::forward<Array>( arr ), mSkipCount );
        }
    }

private:
    int mSkipCount;
};

template<typename Func>
class SkipWhileBuilder
{
    template<class Array>
    class is_called_skip_while
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

        [[nodiscard]] auto end() const noexcept
        {
            return End( mArray );
        }

    private:
        Array mArray;
        Func mFunc;
    };

public:
    explicit SkipWhileBuilder( Func&& func ) noexcept : mFunc( func )
    {
    }

    template<class Array>
    auto build( Array&& arr ) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");
        static_assert(is_called_skip_while<Array>::value, "function result is void or don't call function");

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

inline SkipBuilder Skip( const int cnt ) noexcept
{
    return SkipBuilder( cnt );
}

template<typename Func>
SkipWhileBuilder<Func> SkipWhile( Func&& func ) noexcept
{
    return SkipWhileBuilder<Func>( std::forward<Func>( func ) );
}

}
