#include "Test.h"

TEST( Max, Normal )
{
	const auto baseArray = gBigStruct << SELECT(val, val.id) << linq::ToVector();

	auto max = std::numeric_limits<int>::min();
    for ( auto && value : baseArray )
    {
		if (max < value)
			max = value;
    }

    const auto result = baseArray << linq::Max();
    ASSERT_EQ( max, result );
}

#if ENABLE_PERF
static void NativeMax( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto max = 0.0f;
		for (auto&& value : baseArray)
		{
			if (max < value)
				max = value;
		}
		benchmark::DoNotOptimize(max);
		MEM_COUNTER( state );
	}
}

static void MyLinqMax( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto result = baseArray << linq::Max();
		benchmark::DoNotOptimize(result);
		MEM_COUNTER( state );
	}
}

static void CppLinqMax( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto result = cpplinq::from( baseArray ) >> cpplinq::max();
		benchmark::DoNotOptimize(result);
		MEM_COUNTER( state );
	}
}

BENCHMARK( NativeMax );
BENCHMARK( MyLinqMax );
BENCHMARK( CppLinqMax );
#endif
