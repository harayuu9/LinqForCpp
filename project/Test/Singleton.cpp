#include "Test.h"

TEST( Singleton, Normal )
{
    std::vector<int> range;
    range.push_back( 10 );

    const auto result = linq::Singleton( 10 ) << linq::ToVector();

    ASSERT_EQ( range, result );
}

#if ENABLE_PERF
static void NativeSingleton( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        std::vector<int> range;
        range.push_back( 10 );
        MEM_COUNTER( state );
    }
}

static void CppForLinqSingleton( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto result = linq::Singleton( 10 ) << linq::ToVector();
        MEM_COUNTER( state );
    }
}

static void CppLinqSingleton( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto result = cpplinq::singleton( 10 ) >> cpplinq::to_vector();
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeSingleton );
BENCHMARK( CppForLinqSingleton );
BENCHMARK( CppLinqSingleton );
#endif
