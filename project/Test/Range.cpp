#include "Test.h"

TEST( Range, Normal )
{
    std::vector<int> range;
    range.reserve( 5000 );
    for ( auto i = 10; i < 5010; i++ )
    {
        range.push_back( i );
    }

    const auto result = linq::Range( 10, 5000 ) << linq::ToVector();

    ASSERT_EQ( range, result);
}

#if ENABLE_PERF
static void NativeRange( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        std::vector<int> range;
        range.reserve(5000);
        for (auto i = 10; i < 5010; i++)
        {
            range.push_back(i);
        }
        MEM_COUNTER( state );
    }
}

static void MyLinqRange( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto result = linq::Range(10, 5000) << linq::ToVector();
        MEM_COUNTER( state );
    }
}

static void CppLinqRange( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto empty = cpplinq::range(10, 5000) >> cpplinq::to_vector();
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeRange );
BENCHMARK( MyLinqRange );
BENCHMARK( CppLinqRange );
#endif
