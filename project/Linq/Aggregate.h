#pragma once
#include "TypeTraits.h"
#define AGGREGATE(initValue, variant1, variant2, convert) ::linq::Aggregate(initValue, [&](const auto& (variant1), const auto& (variant2)){ return convert; })

namespace linq {

template<typename Func, typename FuncResult>
struct AggregateBuilder
{
    explicit AggregateBuilder( const FuncResult& initValue, Func&& func ) noexcept : mInitValue( initValue ), mFunc( std::move( func ) )
    {
    }

    template<class Array>
    FuncResult build( Array&& arr ) noexcept
    {
        static_assert(std::is_same_v<FuncResult, array_type_t<Array>>, "aggregate func error");
        static_assert(have_iterator_v<Array> || have_const_iterator_v<Array>, "T have not iterator");

        auto itr = std::begin( arr );
        const auto endItr = std::end( arr );
        if ( itr == endItr )
            return {};
        auto result = mFunc( mInitValue, *itr );
        ++itr;
        for ( ; itr != endItr; ++itr )
        {
            result = mFunc( result, *itr );
        }
        return result;
    }

private:
    FuncResult mInitValue;
    Func mFunc;
};

template<typename Func, typename FuncResult>
AggregateBuilder<Func, FuncResult> Aggregate( const FuncResult initValue, Func&& func ) noexcept
{
    return AggregateBuilder<Func, FuncResult>( initValue, std::forward<Func>( func ) );
}

}
