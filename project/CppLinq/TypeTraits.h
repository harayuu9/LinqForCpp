#pragma once
#include <type_traits>
#include <iterator>
#include "RuntimeMacro.h"

namespace linq {
// have begin() and end()
template<typename T>
class have_iterator
{
    template<typename U> static auto test( int ) -> decltype(
        ( void )std::begin( std::declval<U>() ),
        ( void )std::end( std::declval<U>() ), std::true_type());
    template<typename U> static std::false_type test( ... );
public:
    using Type = decltype(test<T>( 1 ));
};

template<typename T>
struct have_const_iterator
{
private:
    template<typename U> static auto test( int ) -> decltype(
        ( void )std::begin( std::declval<const U>() ),
        ( void )std::end( std::declval<const U>() ), std::true_type());
    template<typename U> static std::false_type test( ... );
public:
    using Type = decltype(test<T>( 1 ));
};

template<typename T>
constexpr bool have_iterator_v = have_iterator<T>::Type::value;
template<typename T>
constexpr bool have_const_iterator_v = have_const_iterator<T>::Type::value;

// Get array type
template<typename Array>
class array_type
{
    class impl1
    {
        template<typename U> static auto test(int) -> decltype(*std::begin(std::declval<std::add_const_t<U>>()));
        template<typename U> static void test(...);
    public:
        using Type = std::remove_const_t<std::remove_reference_t<decltype(test<Array>(1))>>;
    };

    class impl2
    {
        template<typename U> static auto test(int) -> decltype(*std::begin(std::declval<U>()));
        template<typename U> static void test(...);
    public:
        using Type = std::remove_const_t<std::remove_reference_t<decltype(test<Array>(1))>>;
    };

    template<typename U> static auto test( int ) -> decltype(*std::begin( std::declval<std::add_const_t<U>>() ));
    template<typename U> static void test( ... );
public:
    using Type = std::conditional_t<std::is_void_v<typename impl1::Type>, typename impl2::Type, typename impl1::Type>;
};

template<typename Array>
using array_type_t = typename array_type<Array>::Type;

// TODO 分からん。sortの呼び出し条件が決められない。出来たらSortのコスト下げれる
template<typename Array>
class can_sort_array
{
public:
    using Type = std::false_type;
};

template<typename Itr>
constexpr bool can_sort_array_v = false;

// my iterator traits. Easier than STL
template<typename Itr>
class iterator_traits
{
    class get_category
    {
        template<typename U> static auto test( int ) -> typename std::iterator_traits<U>::iterator_category;
        template<typename U> static std::forward_iterator_tag test( ... );
    public:
        using Type = decltype(test<Itr>( 1 ));
    };

    class get_value_type
    {
        template<typename U> static auto test( int ) -> typename std::iterator_traits<U>::value_type;
        template<typename U> static decltype(*std::declval<U>()) test( ... );
    public:
        using Type = decltype(test<Itr>( 1 ));
    };

    class get_difference_type
    {
        template<typename U> static auto test( int ) -> typename std::iterator_traits<U>::difference_type;
        template<typename U> static std::ptrdiff_t test( ... );
    public:
        using Type = decltype(test<Itr>( 1 ));
    };

    class get_pointer
    {
        template<typename U> static auto test( int ) -> typename std::iterator_traits<U>::pointer;
        template<typename U> static auto test( ... ) -> std::add_pointer_t<typename get_value_type::Type>;
    public:
        using Type = decltype(test<Itr>( 1 ));
    };

    class get_reference
    {
        template<typename U> static auto test( int ) -> typename std::iterator_traits<U>::reference;
        template<typename U> static auto test( ... ) -> std::add_lvalue_reference_t<typename get_value_type::Type>;
    public:
        using Type = decltype(test<Itr>( 1 ));
    };

public:
    using iterator_category = typename get_category::Type;
    using value_type = typename get_value_type::Type;
    using difference_type = typename get_difference_type::Type;
    using pointer = typename get_pointer::Type;
    using reference = typename get_reference::Type;
};

template<class Iter>
constexpr bool is_bidi_iter_v = std::is_convertible_v<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>;
template <class Iter>
constexpr bool is_random_iter_v = std::is_convertible_v<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>;

template <class _Kty, class = void>
struct is_nothrow_hashable : std::false_type {};
template <class _Kty>
struct is_nothrow_hashable < _Kty, std::void_t<decltype(std::hash<_Kty>{}(std::declval<const _Kty&>())) >>
    : std::bool_constant < noexcept(std::hash<_Kty>{}(std::declval<const _Kty&>())) > {};

template<class Array, class Builder>
struct is_builder
{
private:
    template<typename T, typename U> static auto test( int ) -> decltype(std::declval<U>().build( std::forward<T>( std::declval<T>() ) ), std::true_type());
    template<typename T, typename U> static std::false_type test( ... );
public:
    using type = decltype(test<Array, Builder>( 0 ));
};

template<class Array, class Builder>
using is_builder_t = std::enable_if_t<is_builder<Array, Builder>::type::value>;

template<class Array>
auto Begin( const Array& arr )
{
    if constexpr ( std::is_pointer_v<Array> )
    {
        if constexpr ( have_iterator_v<Array> )
            return std::begin( *const_cast<Array&>( arr ) );
        else
            return std::begin( *arr );
    }
    else
    {
        if constexpr ( have_iterator_v<Array> )
            return std::begin( const_cast<Array&>( arr ) );
        else
            return std::begin( arr );
    }
}

template<class Array>
auto End( const Array& arr )
{
    if constexpr ( std::is_pointer_v<Array> )
    {
        if constexpr ( have_iterator_v<Array> )
            return std::end( *const_cast<Array&>( arr ) );
        else
            return std::end( *arr );
    }
    else
    {
        if constexpr ( have_iterator_v<Array> )
            return std::end( const_cast<Array&>( arr ) );
        else
            return std::end( arr );
    }
}
}
