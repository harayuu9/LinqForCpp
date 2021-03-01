#include "Test.h"

TEST( Repeat, Normal )
{
    std::vector<int> range;
    range.reserve( 5000 );
    for ( auto i = 0; i < 5000; i++ )
    {
        range.push_back( 10 );
    }

    const auto result = linq::Repeat( 10, 5000 ) << linq::ToVector();

    ASSERT_EQ( range, result);
}

#if ENABLE_PERF
static void NativeRepeat( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        std::vector<int> range;
        range.reserve(5000);
        for (auto i = 0; i < 5000; i++)
        {
            range.push_back(10);
        }
        MEM_COUNTER( state );
    }
}

static void CppForLinqRepeat( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto result = linq::Repeat(10, 5000) << linq::ToVector();
        MEM_COUNTER( state );
    }
}

static void CppLinqRepeat( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto empty = cpplinq::repeat(10, 5000) >> cpplinq::to_vector();
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeRepeat );
BENCHMARK( CppForLinqRepeat );
BENCHMARK( CppLinqRepeat );
#endif
