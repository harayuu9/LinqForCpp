#include "Test.h"

TEST( Contains, Normal )
{
	const auto baseArray = gBigStruct << SELECT(val, val.id) << linq::ToVector();

    ASSERT_TRUE(baseArray << linq::Contains(0));
    ASSERT_FALSE(baseArray << linq::Contains(5000));
}

#if ENABLE_PERF
static void NativeContains( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT(val, val.id) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto itr = std::find(baseArray.begin(), baseArray.end(), 0);
		auto result = itr != baseArray.end() ? *itr : 0;
		itr = std::find(baseArray.begin(), baseArray.end(), 5000);
		result = itr != baseArray.end() ? *itr : 0;
		benchmark::DoNotOptimize(result);
		MEM_COUNTER( state );
	}
}

static void MyLinqContains( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT(val, val.id) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result = baseArray << linq::Contains(0);
		result = baseArray << linq::Contains(5000);
		benchmark::DoNotOptimize(result);
		MEM_COUNTER( state );
	}
}

static void CppLinqContains( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT(val, val.id) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result = cpplinq::from(baseArray) >> cpplinq::contains(0);
		result = cpplinq::from(baseArray) >> cpplinq::contains(5000);
		benchmark::DoNotOptimize(result);
		MEM_COUNTER( state );
	}
}

BENCHMARK( NativeContains );
BENCHMARK( MyLinqContains );
BENCHMARK( CppLinqContains );
#endif
