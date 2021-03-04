#pragma once
#include <algorithm>
#include "TypeTraits.h"

#define ANY(variant, cond) ::linq::Any([&](const auto& (variant)){ return cond; })
namespace linq {

template<typename Func>
struct AnyBuilder
{
    explicit AnyBuilder( Func&& func ) noexcept : mFunc( std::move( func ) )
    {
    }

    template<class Array, typename = std::enable_if_t<std::is_invocable_v<Func, decltype(*Begin( std::declval<Array>() ))>>>
    bool build( Array&& arr ) noexcept
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");
        return std::any_of(std::begin(arr), std::end(arr), mFunc);
    }

private:
    Func mFunc;
};

template<typename Func>
AnyBuilder<Func> Any( Func&& func )
{
    return AnyBuilder<Func>( std::forward<Func>( func ) );
}
}
