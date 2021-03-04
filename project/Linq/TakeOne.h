// ----------------------------------------------------------------------------------------------
// Copyright (c) 2021 Yuto Harada
// ----------------------------------------------------------------------------------------------
// This software is released under the MIT License, see LICENSE.
//
#pragma once
#include "TypeTraits.h"

#ifndef _LIBCPP_NO_EXCEPTIONS
#include <exception>
#define FIRST(variant, cond) ::linq::First([&](const auto& (variant)){ return cond; })
#define LAST(variant, cond) ::linq::Last([&](const auto& (variant)){ return cond; })
#endif

#define LAST_OR_DEFAULT(variant, cond) ::linq::LastOrDefault([&](const auto& (variant)){ return cond; })
#define FIRST_OR_DEFAULT(variant, cond) ::linq::FirstOrDefault([&](const auto& (variant)){ return cond; })
namespace linq {

#ifndef _LIBCPP_NO_EXCEPTIONS
struct empty_exception : std::exception
{
    [[nodiscard]] char const* what() const noexcept override
    {
        return "empty_exception";
    }
};

template<typename Func>
struct FirstBuilder
{
    explicit FirstBuilder( Func&& func ) noexcept : mFunc( std::move( func ) )
    {
    }

    template<class Array>
    array_type_t<Array> build( Array&& arr )
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        auto endItr = std::end(arr);
        auto resultItr = std::find_if(std::begin(arr), endItr, mFunc);
        if (resultItr != endItr)
            return *resultItr;

        throw empty_exception();
    }

private:
    Func mFunc;
};
template<typename Func>
struct LastBuilder
{
    explicit LastBuilder( Func&& func ) noexcept : mFunc( std::move( func ) )
    {
    }

    template<class Array>
    array_type_t<Array> build( Array&& arr )
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        if constexpr (is_bidi_iter_v<decltype(std::begin(arr))>)
        {
            auto begin = std::begin(arr);
            auto itr = --std::end(arr);
            for(;itr != begin; --itr)
            {
                if (mFunc(*itr))
                    return *itr;
            }
            if (mFunc(*itr))
                return *itr;
        }
        else
        {
            array_type_t<Array>* pFind = nullptr;
            for (auto&& value : arr)
            {
                if (mFunc(value))
                {
                    pFind = &value;
                }
            }
            if (pFind)
                return *pFind;
        }
        throw empty_exception();
    }

private:
    Func mFunc;
};

template<typename Func>
FirstBuilder<Func> First(Func&& func) noexcept
{
    return FirstBuilder<Func>(std::forward<Func>(func));
}
template<typename Func>
LastBuilder<Func> Last(Func&& func) noexcept
{
    return LastBuilder<Func>(std::forward<Func>(func));
}
#endif

template<typename Func>
struct FirstOrDefaultBuilder
{
    explicit FirstOrDefaultBuilder(Func&& func) noexcept : mFunc(std::move(func))
    {
    }

    template<class Array>
    array_type_t<Array> build(Array&& arr) noexcept
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        auto endItr = std::end(arr);
        auto resultItr = std::find_if(std::begin(arr), endItr, mFunc);

        if (resultItr != endItr)
            return *resultItr;
        return {};
    }

private:
    Func mFunc;
};

template<typename Func>
struct LastOrDefaultBuilder
{
    explicit LastOrDefaultBuilder( Func&& func ) noexcept : mFunc( std::move( func ) )
    {
    }

    template<class Array>
    array_type_t<Array> build( Array&& arr ) noexcept
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        if constexpr (is_bidi_iter_v<decltype(std::begin(arr))>)
        {
            auto begin = std::begin(arr);
            auto itr = --std::end(arr);
            for (; itr != begin; --itr)
            {
                if (mFunc(*itr))
                    return *itr;
            }
            if (mFunc(*itr))
                return *itr;
        }
        else
        {
            array_type_t<Array>* pFind = nullptr;
            for (auto&& value : arr)
            {
                if (mFunc(value))
                {
                    pFind = &value;
                }
            }
            if (pFind)
                return *pFind;
        }
        return {};
    }

private:
    Func mFunc;
};

template<typename Func>
FirstOrDefaultBuilder<Func> FirstOrDefault(Func&& func) noexcept
{
    return FirstOrDefaultBuilder<Func>(std::forward<Func>(func));
}

template<typename Func>
LastOrDefaultBuilder<Func> LastOrDefault( Func&& func ) noexcept
{
    return LastOrDefaultBuilder<Func>( std::forward<Func>( func ) );
}

}
