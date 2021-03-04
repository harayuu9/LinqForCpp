#include "Test.h"

TEST( Take, Normal )
{
    const auto take = gBigStruct << linq::Take( 10 );
    ASSERT_TRUE( std::equal(take.begin(), take.end(), gBigStruct.begin(), gBigStruct.end()) - 10 );

    // found
    {
        const auto takeWhile = gBigStruct << TAKE_WHILE( val, val.id < 4000 );
        auto itr = gBigStruct.begin();
        for ( ; itr != gBigStruct.end(); ++itr )
        {
            if ( !( itr->id < 4000 ) )
            {
                break;
            }
        }
        ASSERT_TRUE( std::equal(takeWhile.begin(), takeWhile.end(), gBigStruct.begin(), itr) );
    }

    // not found
    {
        const auto takeWhile = gBigStruct << TAKE_WHILE( val, val.id < 10000 );
        auto itr = gBigStruct.begin();
        for ( ; itr != gBigStruct.end(); ++itr )
        {
            if ( !( itr->id < 10000 ) )
            {
                break;
            }
        }
        ASSERT_TRUE( itr == gBigStruct.end() );
        ASSERT_TRUE( std::equal(takeWhile.begin(), takeWhile.end(), gBigStruct.begin(), itr) );
    }
}

#if ENABLE_PERF
static void NativeTake( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        std::vector<CData> take( gBigStruct.begin(), gBigStruct.end() - 10 );
        MEM_COUNTER( state );
    }
}

static void LinqForCppTake( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto take = gBigStruct << linq::Take( 10 ) << linq::ToVector();
        benchmark::DoNotOptimize( take );
        MEM_COUNTER( state );
    }
}

static void CppLinqTake( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = cpplinq::from( gBigStruct ) >> cpplinq::take( 10 ) >> cpplinq::to_vector();
        benchmark::DoNotOptimize( result );
        MEM_COUNTER( state );
    }
}

static void NativeTakeWhile( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto itr = gBigStruct.begin();
        for ( ; itr != gBigStruct.end(); ++itr )
        {
            if ( itr->id > 4000 )
                break;
        }
        std::vector<CData> takeWhile( gBigStruct.begin(), itr );
        MEM_COUNTER( state );
    }
}

static void LinqForCppTakeWhile( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto takeWhile = gBigStruct << TAKE_WHILE( val, val.id > 4000 ) << linq::ToVector();
        benchmark::DoNotOptimize( takeWhile );
        MEM_COUNTER( state );
    }
}

static void CppLinqTakeWhile( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto takeWhile = cpplinq::from( gBigStruct ) >> cpplinq::take_while( [&]( const auto& val ) { return val.id > 4000; } ) >> cpplinq::to_vector();
        benchmark::DoNotOptimize( takeWhile );
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeTake );
BENCHMARK( LinqForCppTake );
BENCHMARK( CppLinqTake );
BENCHMARK( NativeTakeWhile );
BENCHMARK( LinqForCppTakeWhile );
BENCHMARK( CppLinqTakeWhile );
#endif
