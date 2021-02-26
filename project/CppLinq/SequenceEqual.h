#pragma once
#include <algorithm>
#include "TypeTraits.h"

namespace linq {

template<typename Array1>
struct SequenceEqualBuilder
{
    explicit SequenceEqualBuilder( Array1&& value ) noexcept : mArray( std::move( value ) )
    {
    }

    template<typename Array2>
    class is_equal
    {
        template<typename A> static auto test( int )
        -> decltype(*Begin( std::declval<Array1>() ) != *Begin( std::declval<A>() ), std::true_type());
        template<typename A> static auto test( ... ) -> std::false_type;
    public:
        using Type = decltype(test<Array2>( 0 ));
    };

    template<class Array2, typename = std::enable_if_t<is_equal<Array2>::Type::value>>
    bool build( Array2&& arr ) noexcept
    {
        static_assert(have_iterator_v<Array2> || have_const_iterator_v<Array2>, "T have not iterator");

        auto itr1 = Begin( mArray );
        auto itr2 = Begin( arr );
        const auto end1 = End( mArray );
        const auto end2 = End( arr );
        return std::equal(Begin(mArray), End(mArray), Begin(arr), End(arr));
    }

private:
    Array1 mArray;
};

template<typename Array>
auto SequenceEqual( Array&& arr )
{
    if constexpr ( std::is_lvalue_reference_v<Array> )
    {
        return SequenceEqualBuilder<decltype(&arr)>( &arr );
    }
    else
    {
        return SequenceEqualBuilder<Array>( std::forward<Array>( arr ) );
    }
}
}
