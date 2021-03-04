// ----------------------------------------------------------------------------------------------
// Copyright (c) 2021 Yuto Harada
// ----------------------------------------------------------------------------------------------
// This software is released under the MIT License, see LICENSE.
//
#pragma once
#include <vector>
#include "TypeTraits.h"

namespace linq {

class ReverseBuilder
{
    template<class Array>
    struct BidirectionalImpl
    {
        using RawItr = decltype(Begin(std::declval<Array>()));
        explicit BidirectionalImpl(Array&& arr) noexcept : mArray( std::move( arr ) )
        {
        }

        [[nodiscard]] auto begin() const noexcept
        {
            return std::make_reverse_iterator(End(mArray));
        }

        auto begin() noexcept
        {
            return std::make_reverse_iterator(End(mArray));
        }

        [[nodiscard]] auto end() const noexcept
        {
            return std::make_reverse_iterator(Begin(mArray));
        }

        auto end() noexcept
        {
            return std::make_reverse_iterator(Begin(mArray));
        }

    private:
        Array mArray;
    };
public:
    template<class Array>
    auto build(Array&& arr)
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        if constexpr (is_bidi_iter_v<decltype(Begin(arr))>)
        {
            if constexpr (std::is_lvalue_reference_v<Array>)
            {
                return BidirectionalImpl<decltype(&arr)>(&arr);
            }
            else
            {
                return BidirectionalImpl<Array>(std::forward<Array>(arr));
            }
        }
        else
        {
            std::vector<array_type_t<Array>, Allocator<array_type_t<Array>>> tmp(Begin(arr), End(arr));
            std::reverse(tmp.begin(), tmp.end());
            return tmp;
        }
    }
};

inline ReverseBuilder Reverse()
{
    return ReverseBuilder();
}

}