#include "Test.h"

TEST( Any, Normal )
{
	auto baseResult = std::any_of(std::begin(gBigStruct), std::end(gBigStruct), [](const auto& val) {return val.id > 0; });
	auto result = gBigStruct << ANY(val, val.id > 0);
    ASSERT_EQ( baseResult, result );

	baseResult = std::any_of(std::begin(gBigStruct), std::end(gBigStruct), [](const auto& val) {return val.id > 50; });
	result = gBigStruct << ANY(val, val.id > 50);
	ASSERT_EQ(baseResult, result);
}

#if ENABLE_PERF
static void NativeAny( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto baseResult = std::any_of(std::begin(gBigStruct), std::end(gBigStruct), [](const auto& val) {return val.id > 0; });
		baseResult = std::any_of(std::begin(gBigStruct), std::end(gBigStruct), [](const auto& val) {return val.id > 50; });
		MEM_COUNTER( state );
	}
}

static void LinqForCppAny( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result = gBigStruct << ANY(val, val.id > 0);
		result = gBigStruct << ANY(val, val.id > 50);
		MEM_COUNTER( state );
	}
}

static void CppLinqAny( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result = cpplinq::from(gBigStruct) >> cpplinq::any([](const auto& val) {return val.id > 0; });
		result = cpplinq::from(gBigStruct) >> cpplinq::any([](const auto& val) {return val.id > 50; });
		MEM_COUNTER( state );
	}
}

BENCHMARK( NativeAny );
BENCHMARK( LinqForCppAny );
BENCHMARK( CppLinqAny );
#endif
