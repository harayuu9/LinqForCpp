﻿// ----------------------------------------------------------------------------------------------
// Copyright (c) 2021 Yuto Harada
// ----------------------------------------------------------------------------------------------
// This software is released under the MIT License, see LICENSE.
//
#pragma once
#include "TypeTraits.h"

namespace linq {

struct Min
{
    template<class Array>
    array_type_t<Array> build( Array&& arr )
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        auto itr = std::begin( arr );
        const auto endItr = std::end( arr );
        if ( itr == endItr )
            return {};
        auto result = *itr;
        ++itr;
        for ( ; itr != endItr; ++itr )
        {
            if ( result > *itr )
                result = *itr;
        }
        return result;
    }
};
}
