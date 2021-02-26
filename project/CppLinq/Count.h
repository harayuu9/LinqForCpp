#pragma once
#include "TypeTraits.h"

#define COUNT(variant, cond) ::linq::Count([&](const auto& (variant)){ return cond; })
namespace linq {

struct CountNonFuncBuilder
{
    template<class Array>
    struct is_get_size
    {
    private:
        template<typename U> static auto test( int ) -> decltype(std::declval<U>().size(), std::true_type());
        template<typename U> static std::false_type test( ... );
    public:
        using Type = decltype(test<Array>( 0 ));
    };

    template<class Array>
    auto build( Array&& arr )
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        if constexpr ( is_get_size<Array>::Type::value )
        {
            return arr.size();
        }
        else
        {
            return static_cast<std::size_t>( std::distance( std::begin( arr ), std::end( arr ) ) );
        }
    }
};

template<typename Func>
struct CountBuilder
{
    explicit CountBuilder( Func&& func ) noexcept : mFunc( std::move( func ) )
    {
    }

    template<class Array>
    std::size_t build( Array&& arr ) noexcept
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        std::size_t result = 0;
        for ( auto&& value : arr )
        {
            if ( mFunc( value ) )
            {
                result++;
            }
        }
        return result;
    }

private:
    Func mFunc;
};

template<typename Func>
CountBuilder<Func> Count( Func&& func ) noexcept
{
    return CountBuilder<Func>( std::forward<Func>( func ) );
}

inline CountNonFuncBuilder Count() noexcept
{
    return CountNonFuncBuilder();
}
}
