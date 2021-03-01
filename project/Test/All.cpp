#include "Test.h"

TEST( All, Normal )
{
	auto baseResult = std::all_of(std::begin(gBigStruct), std::end(gBigStruct), [](const auto& val) {return val.id > 0; });
	auto result = gBigStruct << ALL(val, val.id > 0);
    ASSERT_EQ( baseResult, result );

	baseResult = std::all_of(std::begin(gBigStruct), std::end(gBigStruct), [](const auto& val) {return val.id > 50; });
	result = gBigStruct << ALL(val, val.id > 50);
	ASSERT_EQ(baseResult, result);
}

#if ENABLE_PERF
static void NativeAll( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto baseResult = std::all_of(std::begin(gBigStruct), std::end(gBigStruct), [](const auto& val) {return val.id > 0; });
		baseResult = std::all_of(std::begin(gBigStruct), std::end(gBigStruct), [](const auto& val) {return val.id > 50; });
		MEM_COUNTER( state );
	}
}

static void CppForLinqAll( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result = gBigStruct << ALL(val, val.id > 0);
		result = gBigStruct << ALL(val, val.id > 50);
		MEM_COUNTER( state );
	}
}

static void CppLinqAll( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result = cpplinq::from(gBigStruct) >> cpplinq::all([](const auto& val) {return val.id > 0; });
		result = cpplinq::from(gBigStruct) >> cpplinq::all([](const auto& val) {return val.id > 50; });
		MEM_COUNTER( state );
	}
}

BENCHMARK( NativeAll );
BENCHMARK( CppForLinqAll );
BENCHMARK( CppLinqAll );
#endif
