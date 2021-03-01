#include "Test.h"

TEST( Reverse, Normal )
{
	std::vector<CData> data(gBigStruct.size());
	std::reverse_copy(gBigStruct.begin(), gBigStruct.end(), data.begin());

    ASSERT_TRUE( gBigStruct << linq::Reverse() << linq::ToVector() == data);
}

#if ENABLE_PERF
static void NativeReverse( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		std::vector<CData> data(gBigStruct.size());
		std::reverse_copy(gBigStruct.begin(), gBigStruct.end(), data.begin());
		MEM_COUNTER( state );
	}
}

static void CppForLinqReverse( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto result = gBigStruct << linq::Reverse() << linq::ToVector();
		MEM_COUNTER( state );
	}
}

static void CppLinqReverse( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result = cpplinq::from(gBigStruct) >> cpplinq::reverse() >> cpplinq::to_vector();
		MEM_COUNTER( state );
	}
}
BENCHMARK( NativeReverse );
BENCHMARK( CppForLinqReverse );
BENCHMARK( CppLinqReverse );
#endif
