#pragma once
#include "TypeTraits.h"

namespace linq {

template<typename Result>
struct Avg
{
    template<class Array>
    Result build( Array&& arr )
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");
        static_assert(std::is_convertible_v<array_type_t<Array>, Result>, "array type to result. convert error");

        auto itr = std::begin( arr );
        const auto endItr = std::end( arr );
        if ( itr == endItr )
            return {};
        auto result = static_cast<Result>( *itr );
        ++itr;
        if constexpr (is_random_iter_v<decltype(itr)>)
        {
            for (; itr != endItr; ++itr)
            {
                result += static_cast<Result>(*itr);
            }
            return result / std::distance( std::begin(arr), endItr );
        }
        else
        {
            auto elemCnt = 1;
            for (; itr != endItr; ++itr)
            {
                result += static_cast<Result>(*itr);
                elemCnt++;
            }
            return result / elemCnt;
        }
    }
};

}
