#include <fstream>
#include <iostream>
#include "Test.h"

#if ENABLE_PERF
static void NativeDistinct( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        std::vector<CData> result;
        for ( auto&& bigStruct : gBigStruct )
        {
            if ( std::find( result.begin(), result.end(), bigStruct ) == result.end() )
                result.push_back( bigStruct );
        }
        MEM_COUNTER( state );
    }
}


static void MyLinqDistinct( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        // distinct : 重複を取り除く。
        auto _Ret5 = gBigStruct
            << linq::Distinct()
            << linq::ToVector();
        MEM_COUNTER( state );
    }
}

static void CppLinqDistinct( benchmark::State& state )
{
    using namespace cpplinq;

    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto _Ret2 = from( gBigStruct )
            >> distinct()
            >> to_vector();
        MEM_COUNTER( state );
    }
}

static void NativeSelectMany( benchmark::State& state )
{
    auto smallStruct = gBigStruct << linq::Take( 10 ) << linq::ToVector();
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        std::vector<int> result;
        for ( auto&& data : smallStruct )
        {
            std::vector<int> temp;
            temp.reserve( data.id );
            for ( auto i = 0; i < data.id; i++ )
            {
                temp.push_back( data.id * i );
            }
            result.insert( result.end(), temp.begin(), temp.end() );
        }
        MEM_COUNTER( state );
    }
}

static void MyLinqSelectMany( benchmark::State& state )
{
    using namespace linq;

    auto smallStruct = gBigStruct << Take( 10 ) << ToVector();
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto _Ret2 = smallStruct
            << SelectMany( [&]( const auto& x )
            {
                std::vector<int> result;
                result.reserve( x.id );
                for ( auto i = 0; i < x.id; i++ )
                {
                    result.push_back( x.id * i );
                }
                return result;
            } )
            << ToVector();
        MEM_COUNTER( state );
    }
}

static void MyLinqOrderBy( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = gBigStruct
            << linq::OrderByAscending( []( const auto& data ) { return data.id; } )
            << linq::ThenByDescending( []( const auto& x ) { return x.name.length(); } )
            << linq::ToVector();
        MEM_COUNTER( state );
    }
}

static void CppLinqOrderBy( benchmark::State& state )
{
    using namespace cpplinq;
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto _Ret1 = from( gBigStruct )
            >> orderby_ascending( []( const auto& x ) { return ( x.id ); } )
            >> thenby_descending( []( const auto& x ) { return ( x.name.length() ); } )
            >> to_vector();
        MEM_COUNTER( state );
    }
}

static void NativeFirstOrDefault( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto _Ret5 = std::find_if( std::begin( gBigStruct ), std::end( gBigStruct ),
                                   []( const CData& data ) { return data.id == 10; } );
        if ( _Ret5 != std::end( gBigStruct ) )
        {
            benchmark::DoNotOptimize(*_Ret5);
        }
        MEM_COUNTER( state );
    }
}

static void MyLinqFirstOrDefault( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto _Ret5 = gBigStruct
            << linq::FirstOrDefault( []( const CData& data ) { return data.id == 10; } );
        MEM_COUNTER( state );
    }
}

static void CppLinqFirstOrDefault( benchmark::State& state )
{
    using namespace cpplinq;

    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto _Ret2 = from( gBigStruct )
            >> first_or_default( []( const CData& data ) { return data.id == 10; } );
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeSelectMany );
BENCHMARK( NativeSelectMany );
BENCHMARK( MyLinqSelectMany );
BENCHMARK( NativeDistinct );
BENCHMARK( MyLinqDistinct );
BENCHMARK( CppLinqDistinct );
BENCHMARK( MyLinqOrderBy );
BENCHMARK( CppLinqOrderBy );
BENCHMARK( NativeFirstOrDefault );
BENCHMARK( MyLinqFirstOrDefault );
BENCHMARK( CppLinqFirstOrDefault );

#endif