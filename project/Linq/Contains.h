#pragma once
#include "TypeTraits.h"

namespace linq {

template<typename T>
struct ContainsBuilder
{
    explicit ContainsBuilder( const T& value ) noexcept : mValue( value )
    {
    }

    template<typename Array>
    class is_equal
    {
        template<typename A> static auto test( int )
        -> decltype(std::declval<T>() == *Begin( std::declval<A>() ), std::true_type());
        template<typename A> static auto test( ... ) -> std::false_type;
    public:
        using Type = decltype(test<Array>( 0 ));
    };

    template<class Array, typename = std::enable_if_t<is_equal<Array>::Type::value>>
    bool build( Array&& arr ) noexcept
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");
        for ( auto&& value : arr )
        {
            if ( value == mValue )
                return true;
        }
        return false;
    }

private:
    T mValue;
};

template<typename T>
ContainsBuilder<T> Contains( const T& value )
{
    return ContainsBuilder<T>( value );
}
}
