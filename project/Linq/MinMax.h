#pragma once
#include "TypeTraits.h"

namespace linq {

struct MinMax
{
    template<class Array>
    std::pair<array_type_t<Array>, array_type_t<Array>> build( Array&& arr )
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        auto itr = std::begin( arr );
        const auto endItr = std::end( arr );
        if ( itr == endItr )
            return {};
        auto min = *itr;
        auto max = *itr;
        ++itr;
        for ( ; itr != endItr; ++itr )
        {
            if ( min > *itr )
                min = *itr;
            if ( max < *itr )
                max = *itr;
        }
        return std::make_pair( min, max );
    }
};

}
