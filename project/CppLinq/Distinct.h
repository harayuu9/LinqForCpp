#pragma once
#include <unordered_set>
#include "TypeTraits.h"
#include "Allocator.h"

namespace linq {
class DistinctBuilder
{
public:
    template<class Array>
    auto build( Array&& arr ) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");

        using ValueType = array_type_t<Array>;
        using SetType = std::conditional_t<is_nothrow_hashable<ValueType>::value,
            std::unordered_set<ValueType, std::hash<ValueType>, std::equal_to<>, Allocator<ValueType>>,
            std::set<ValueType, std::less<>, Allocator<ValueType>>>;

        return SetType(std::begin(arr), std::end(arr));
    }
};

inline DistinctBuilder Distinct() noexcept
{
    return DistinctBuilder();
}
}
