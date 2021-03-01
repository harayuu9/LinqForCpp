#include "Test.h"

TEST( Avg, Normal )
{
	const auto baseArray = gBigStruct << SELECT(val, val.id) << linq::ToVector();

    auto baseResult = 0.0f;
    for ( auto && value : baseArray )
    {
		baseResult += static_cast<float>(value);
    }
	baseResult /= baseArray.size();

    const auto result = baseArray << linq::Avg<float>();
    ASSERT_EQ( baseResult, result );
}

#if ENABLE_PERF
static void NativeAvg( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto baseResult = 0.0f;
		for (auto&& value : baseArray)
		{
			baseResult += static_cast<float>(value);
		}
		baseResult /= baseArray.size();
		benchmark::DoNotOptimize(baseResult);
		MEM_COUNTER( state );
	}
}

static void CppForLinqAvg( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto result = baseArray << linq::Avg<float>();
		benchmark::DoNotOptimize(result);
		MEM_COUNTER( state );
	}
}

static void CppLinqAvg( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto result = cpplinq::from( baseArray) >> cpplinq::avg();
		benchmark::DoNotOptimize(result);
		MEM_COUNTER( state );
	}
}

BENCHMARK( NativeAvg );
BENCHMARK( CppForLinqAvg );
BENCHMARK( CppLinqAvg );
#endif
