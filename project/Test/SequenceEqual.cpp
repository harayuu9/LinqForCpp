#include "Test.h"

TEST( SequenceEqual, Normal )
{
    auto reverse = gBigStruct << linq::Reverse();
    ASSERT_TRUE( gBigStruct << linq::SequenceEqual(gBigStruct) );
    ASSERT_FALSE( gBigStruct << linq::SequenceEqual(reverse) );
    ASSERT_TRUE( reverse << linq::SequenceEqual(reverse) );
}

#if ENABLE_PERF
static void NativeSequenceEqual( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        benchmark::DoNotOptimize( gBigStruct == gBigStruct );
        MEM_COUNTER( state );
    }
}

static void CppForLinqSequenceEqual( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto result = gBigStruct << linq::SequenceEqual( gBigStruct );
        benchmark::DoNotOptimize( result );
        MEM_COUNTER( state );
    }
}

static void CppLinqSequenceEqual( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = cpplinq::from( gBigStruct ) >> cpplinq::sequence_equal( cpplinq::from( gBigStruct ) );
        benchmark::DoNotOptimize( result );
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeSequenceEqual );
BENCHMARK( CppForLinqSequenceEqual );
BENCHMARK( CppLinqSequenceEqual );
#endif
