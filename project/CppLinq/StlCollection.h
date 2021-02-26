#pragma once
#include "TypeTraits.h"
#include "Allocator.h"
#include <vector>
#include <list>

namespace linq {

struct ToVectorBuilder
{
    template<class Array>
    auto build(Array&& arr) noexcept
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        if constexpr (is_select_many_v<Array>)
        {
            std::vector<array_type_t<Array>> result;
            auto begin = Begin(arr.mArray);
            const auto end = End(arr.mArray);

            for(auto itr = begin; itr != end;++itr)
            {
                const auto funcResArr = arr.mFunc(*itr);
                result.insert(result.end(), std::begin(funcResArr), std::end(funcResArr));
            }
            return result;
        }
        else if constexpr (get_order_by_type_v<Array> == 0) // OrderByType = std::vector
        {
            return std::move(arr.mIndexArray);
        }
        else
        {
            return std::vector<array_type_t<Array>, Allocator<array_type_t<Array>>>( std::begin(arr), std::end(arr) );
        }
    }
};
struct ToListBuilder
{
    template<class Array>
    std::list<array_type_t<Array>, Allocator<array_type_t<Array>>> build(Array&& arr) noexcept
    {
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        if constexpr (is_select_many_v<Array>)
        {
            std::list<array_type_t<Array>> result;
            auto begin = Begin(arr.mArray);
            const auto end = End(arr.mArray);

            for (auto itr = begin; itr != end; ++itr)
            {
                const auto funcResArr = arr.mFunc(*itr);
                result.insert(result.end(), std::begin(funcResArr), std::end(funcResArr));
            }
            return result;
        }
        else
        {
            return { std::begin(arr), std::end(arr) };
        }
    }
};

inline ToVectorBuilder ToVector() noexcept
{
    return ToVectorBuilder();
}
inline ToListBuilder ToList() noexcept
{
    return ToListBuilder();
}
}
