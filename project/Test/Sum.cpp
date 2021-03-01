#include "Test.h"

TEST( Sum, Normal )
{
    const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();

    auto baseResult = 0;
    for ( auto&& value : baseArray )
    {
        baseResult += value;
    }

    const auto result = baseArray << linq::Sum();
    ASSERT_EQ( baseResult, result );
}

#if ENABLE_PERF
static void NativeSum( benchmark::State& state )
{
    const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto baseResult = 0.0f;
        for ( auto&& value : baseArray )
        {
            baseResult += static_cast<float>( value );
        }
        benchmark::DoNotOptimize( baseResult );
        MEM_COUNTER( state );
    }
}

static void LinqForCppSum( benchmark::State& state )
{
    const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto result = baseArray << linq::Sum();
        benchmark::DoNotOptimize( result );
        MEM_COUNTER( state );
    }
}

static void CppLinqSum( benchmark::State& state )
{
    const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto result = cpplinq::from( baseArray ) >> cpplinq::sum();
        benchmark::DoNotOptimize( result );
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeSum );
BENCHMARK( LinqForCppSum );
BENCHMARK( CppLinqSum );
#endif
