// ----------------------------------------------------------------------------------------------
// Copyright (c) 2021 Yuto Harada
// ----------------------------------------------------------------------------------------------
// This software is released under the MIT License, see LICENSE.
//
#pragma once
#include "OrderBy.h"

#define THEN_BY(variant, cond, isAscending) ::linq::ThenBy([&](const auto& (variant)){ return cond;}, isAscending)
#define THEN_BY_ASCENDING(variant, cond)    ::linq::ThenByAscending([&](const auto& (variant)){ return cond;})
#define THEN_BY_DESCENDING(variant, cond)   ::linq::ThenByDescending([&](const auto& (variant)){ return cond;})
namespace linq {

template<typename Func>
class ThenByBuilder
{
public:
    ThenByBuilder( Func&& func, const bool isAscending ) noexcept : mFunc( std::move( func ) ), mIsAscending( isAscending )
    {
    }

    template<class Array, typename = std::enable_if_t<get_order_by_type_v<Array> != -1>>
    auto build( Array&& arr ) noexcept
    {
        if constexpr ( get_order_by_type_v<Array> == 0 )
        {
            sort_( arr.mIndexArray, arr.mFunc );
        }
        else if constexpr ( get_order_by_type_v<Array> == 1 )
        {
            sort_( arr.mArray, arr.mFunc );
        }
        return std::forward<Array>( arr );
    }

private:
    template<class Array, typename F>
    void sort_( Array& arr, const F& func ) noexcept
    {
        auto itr = Begin( arr );
        auto end = End( arr );
        if ( itr == end )
            return;
        auto startItr = itr;
        auto beforeValue = func( *itr );
        ++itr;

        auto ascending = [&]( const auto& itr1, const auto& itr2 )
        {
            return mFunc( itr1 ) < mFunc( itr2 );
        };
        auto descending = [&]( const auto& itr1, const auto& itr2 )
        {
            return mFunc( itr1 ) > mFunc( itr2 );
        };

        for ( ; itr != end; ++itr )
        {
            auto value = func( *itr );
            if ( value != beforeValue )
            {
                if ( mIsAscending )
                    std::sort( startItr, itr, ascending );
                else
                    std::sort( startItr, itr, descending );
                startItr = itr;
                beforeValue = value;
            }
        }
        if ( mIsAscending )
            std::sort( startItr, end, ascending );
        else
            std::sort( startItr, end, descending );
    }

    Func mFunc;
    bool mIsAscending;
};

template<typename Func>
ThenByBuilder<Func> ThenBy( Func&& func, const bool isAscending = true )
{
    return ThenByBuilder<Func>( std::forward<Func>( func ), isAscending );
}

template<typename Func>
ThenByBuilder<Func> ThenByAscending( Func&& func )
{
    return ThenByBuilder<Func>( std::forward<Func>( func ), true );
}

template<typename Func>
ThenByBuilder<Func> ThenByDescending( Func&& func )
{
    return ThenByBuilder<Func>( std::forward<Func>( func ), false );
}


}
