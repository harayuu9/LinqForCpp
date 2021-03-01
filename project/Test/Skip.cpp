#include "Test.h"

TEST( Skip, Normal )
{
    const auto skip = gBigStruct << linq::Skip( 10 );
    ASSERT_TRUE( std::equal(skip.begin(), skip.end(), gBigStruct.begin() + 10, gBigStruct.end()) );

    // found
    {
        const auto skipWhile = gBigStruct << SKIP_WHILE( val, val.id < 4000 );
        auto itr = gBigStruct.begin();
        for ( ; itr != gBigStruct.end(); ++itr )
        {
            if ( !( itr->id < 4000 ) )
            {
                ++itr;
                break;
            }
        }
        ASSERT_TRUE( std::equal(skipWhile.begin(), skipWhile.end(), itr, gBigStruct.end()) );
    }

    // not found
    {
        const auto skipWhile = gBigStruct << SKIP_WHILE( val, val.id < 10000 );
        auto itr = gBigStruct.begin();
        for ( ; itr != gBigStruct.end(); ++itr )
        {
            if ( !( itr->id < 10000 ) )
            {
                ++itr;
                break;
            }
        }
        ASSERT_TRUE( itr == gBigStruct.end() );
        ASSERT_TRUE( skipWhile.begin() == skipWhile.end() );
        ASSERT_TRUE( std::equal(skipWhile.begin(), skipWhile.end(), itr, gBigStruct.end()) );
    }
}

#if ENABLE_PERF
static void NativeSkip( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        std::vector<CData> skip( gBigStruct.begin() + 10, gBigStruct.end() );
        MEM_COUNTER( state );
    }
}

static void CppForLinqSkip( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto skip = gBigStruct << linq::Skip( 10 ) << linq::ToVector();
        benchmark::DoNotOptimize( skip );
        MEM_COUNTER( state );
    }
}

static void CppLinqSkip( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = cpplinq::from( gBigStruct ) >> cpplinq::skip( 10 ) >> cpplinq::to_vector();
        benchmark::DoNotOptimize( result );
        MEM_COUNTER( state );
    }
}

static void NativeSkipWhile( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto itr = gBigStruct.begin();
        for ( ; itr != gBigStruct.end(); ++itr )
        {
            if ( itr->id > 4000 )
            {
                ++itr;
                break;
            }
        }
        std::vector<CData> skipWhile( itr, gBigStruct.end() );
        MEM_COUNTER( state );
    }
}

static void CppForLinqSkipWhile( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto skipWhile = gBigStruct << SKIP_WHILE( val, val.id > 4000 ) << linq::ToVector();
        benchmark::DoNotOptimize( skipWhile );
        MEM_COUNTER( state );
    }
}

static void CppLinqSkipWhile( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto skipWhile = cpplinq::from( gBigStruct ) >> cpplinq::skip_while( [&]( const auto& val ) { return val.id > 4000; } ) >> cpplinq::to_vector();
        benchmark::DoNotOptimize( skipWhile );
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeSkip );
BENCHMARK( CppForLinqSkip );
BENCHMARK( CppLinqSkip );
BENCHMARK( NativeSkipWhile );
BENCHMARK( CppForLinqSkipWhile );
BENCHMARK( CppLinqSkipWhile );
#endif
