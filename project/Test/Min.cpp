#include "Test.h"

TEST( Min, Normal )
{
	const auto baseArray = gBigStruct << SELECT(val, val.id) << linq::ToVector();

    auto min = std::numeric_limits<int>::max();
    for ( auto && value : baseArray )
    {
		if (min > value)
			min = value;
    }

    const auto result = baseArray << linq::Min();
    ASSERT_EQ( min, result );
}

#if ENABLE_PERF
static void NativeMin( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto min = 0.0f;
		for (auto&& value : baseArray)
		{
			if (min < value)
				min = value;
		}
		benchmark::DoNotOptimize(min);
		MEM_COUNTER( state );
	}
}

static void CppForLinqMin( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto result = baseArray << linq::Min();
		benchmark::DoNotOptimize(result);
		MEM_COUNTER( state );
	}
}

static void CppLinqMin( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto result = cpplinq::from( baseArray ) >> cpplinq::min();
		benchmark::DoNotOptimize(result);
		MEM_COUNTER( state );
	}
}

BENCHMARK( NativeMin );
BENCHMARK( CppForLinqMin );
BENCHMARK( CppLinqMin );
#endif
