#include "Test.h"

TEST( TakeOne, First )
{
    ASSERT_ANY_THROW( gBigStruct << FIRST(val, val.id == 5000) );

    const auto first = gBigStruct << FIRST( val, val.id > 3000 );
    const auto itr = std::find_if( gBigStruct.begin(), gBigStruct.end(), []( const auto& val ) { return val.id > 3000; } );
    ASSERT_EQ( *itr, first );
}

TEST( TakeOne, FirstOrDefault )
{
    ASSERT_NO_THROW( gBigStruct << FIRST_OR_DEFAULT(val, val.id == 5000) );

    const auto firstOrDefault = gBigStruct << FIRST_OR_DEFAULT( val, val.id > 3000 );
    const auto itr = std::find_if( gBigStruct.begin(), gBigStruct.end(), []( const auto& val ) { return val.id > 3000; } );
    ASSERT_EQ( *itr, firstOrDefault );
}

TEST( TakeOne, Last )
{
    ASSERT_ANY_THROW( gBigStruct << LAST(val, val.id == 5000) );

    const auto last = gBigStruct << LAST( val, val.id > 3000 );

    auto result = gBigStruct.begin();
    for ( auto itr = gBigStruct.begin(); itr != gBigStruct.end(); ++itr )
    {
        if ( itr->id > 3000 )
            result = itr;
    }
    ASSERT_EQ( *result, last );
}

TEST( TakeOne, LastOrDefault )
{
    ASSERT_NO_THROW( gBigStruct << LAST_OR_DEFAULT(val, val.id == 5000) );

    const auto lastOrDefault = gBigStruct << LAST_OR_DEFAULT( val, val.id > 3000 );

    auto result = gBigStruct.begin();
    for ( auto itr = gBigStruct.begin(); itr != gBigStruct.end(); ++itr )
    {
        if ( itr->id > 3000 )
            result = itr;
    }
    ASSERT_EQ( *result, lastOrDefault );
}

#if ENABLE_PERF
static void NativeFirst( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto itr = std::find_if( gBigStruct.begin(), gBigStruct.end(), []( const auto& val ) { return val.id > 3000; } );
        benchmark::DoNotOptimize( itr );
        MEM_COUNTER( state );
    }
}

static void CppForLinqFirst( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto first = gBigStruct << FIRST( val, val.id > 3000 );
        benchmark::DoNotOptimize( first );
        MEM_COUNTER( state );
    }
}

static void CppLinqFirst( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = cpplinq::from( gBigStruct ) >> cpplinq::first( []( const auto& val ) { return val.id > 3000; } );
        benchmark::DoNotOptimize( result );
        MEM_COUNTER( state );
    }
}

static void NativeLast( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = gBigStruct.begin();
        for ( auto itr = gBigStruct.begin(); itr != gBigStruct.end(); ++itr )
        {
            if ( itr->id > 3000 )
                result = itr;
        }
        benchmark::DoNotOptimize( result );
        MEM_COUNTER( state );
    }
}

static void CppForLinqLast( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto last = gBigStruct << LAST( val, val.id > 3000 );
        benchmark::DoNotOptimize( last );
        MEM_COUNTER( state );
    }
}

static void CppLinqLast( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto last = cpplinq::from( gBigStruct ) >> cpplinq::last_or_default( [&]( const auto& val ) { return val.id > 3000; } );
        benchmark::DoNotOptimize( last );
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeFirst );
BENCHMARK( CppForLinqFirst );
BENCHMARK( CppLinqFirst );
BENCHMARK( NativeLast );
BENCHMARK( CppForLinqLast );
BENCHMARK( CppLinqLast );
#endif
