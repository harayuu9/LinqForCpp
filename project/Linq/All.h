// ----------------------------------------------------------------------------------------------
// Copyright (c) 2021 Yuto Harada
// ----------------------------------------------------------------------------------------------
// This software is released under the MIT License, see LICENSE.
//
#pragma once
#include <algorithm>
#include "TypeTraits.h"

#define ALL(variant, cond) ::linq::All([&](const auto& (variant)){ return cond; })
namespace linq {

template<typename Func>
struct AllBuilder
{
    explicit AllBuilder( Func&& func ) noexcept : mFunc( std::move( func ) )
    {
    }

    template<class Array, typename = std::enable_if_t<std::is_invocable_v<Func, decltype(*Begin( std::declval<Array>() ))>>>
    bool build( Array&& arr ) noexcept
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");
        return std::all_of(std::begin(arr), std::end(arr), mFunc);
    }

private:
    Func mFunc;
};

template<typename Func>
AllBuilder<Func> All( Func&& func )
{
    return AllBuilder<Func>( std::forward<Func>( func ) );
}
}
