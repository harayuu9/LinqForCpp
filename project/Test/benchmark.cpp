#include <fstream>
#include <iostream>
#include "Test.h"

#if ENABLE_PERF

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
BENCHMARK( NativeFirstOrDefault );
BENCHMARK( MyLinqFirstOrDefault );
BENCHMARK( CppLinqFirstOrDefault );

#endif