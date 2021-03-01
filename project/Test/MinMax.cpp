#include "Test.h"

TEST( MinMax, Normal )
{
	const auto baseArray = gBigStruct << SELECT(val, val.id) << linq::ToVector();

    auto min = std::numeric_limits<int>::max();
    auto max = std::numeric_limits<int>::min();
    for ( auto && value : baseArray )
    {
		if (min > value)
			min = value;
		if (max < value)
			max = value;
    }

    const auto result = baseArray << linq::MinMax();
	ASSERT_EQ(std::make_pair(min, max), result);
}

#if ENABLE_PERF
static void NativeMinMax( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto min = std::numeric_limits<int>::max();
		auto max = std::numeric_limits<int>::min();
		for (auto&& value : baseArray)
		{
			if (min > value)
				min = value;
			if (max < value)
				max = value;
		}
		benchmark::DoNotOptimize(min);
		benchmark::DoNotOptimize(max);
		MEM_COUNTER( state );
	}
}

static void LinqForCppMinMax( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto result = baseArray << linq::MinMax();
		benchmark::DoNotOptimize(result);
		MEM_COUNTER( state );
	}
}

static void CppLinqMinMax( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto min = cpplinq::from( baseArray ) >> cpplinq::min();
		const auto max = cpplinq::from( baseArray ) >> cpplinq::max();
		benchmark::DoNotOptimize(min);
		benchmark::DoNotOptimize(max);
		MEM_COUNTER( state );
	}
}

BENCHMARK( NativeMinMax );
BENCHMARK( LinqForCppMinMax );
BENCHMARK( CppLinqMinMax );
#endif
