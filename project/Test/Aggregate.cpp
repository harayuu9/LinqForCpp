#include "Test.h"

TEST( Aggregate, Normal )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();

	auto baseResult = 0;
	for ( auto&& value : baseArray )
	{
		baseResult = baseResult * baseResult + value * value;
	}

	const auto result = baseArray << AGGREGATE( 0, a, b, a * a + b * b );
	ASSERT_EQ( baseResult, result );
}

#if ENABLE_PERF
static void NativeAggregate( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto baseResult = 0;
		for ( auto&& value : baseArray )
		{
			baseResult = baseResult * baseResult + value * value;
		}
		MEM_COUNTER( state );
	}
}

static void CppForLinqAggregate( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto result = baseArray << AGGREGATE( 0, a, b, a * a + b * b );
		MEM_COUNTER( state );
	}
}

static void CppLinqAggregate( benchmark::State& state )
{
	const auto baseArray = gBigStruct << SELECT( val, val.id ) << linq::ToVector();
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result = cpplinq::from( baseArray ) >> cpplinq::aggregate( 0, []( auto& a, auto& b ) {
			return a * a + b * b;
		} );
		MEM_COUNTER( state );
	}
}

BENCHMARK( NativeAggregate );
BENCHMARK( CppForLinqAggregate );
BENCHMARK( CppLinqAggregate );
#endif
