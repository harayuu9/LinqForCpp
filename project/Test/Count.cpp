#include "Test.h"

TEST( Count, Normal )
{
    ASSERT_TRUE( gBigStruct << linq::Count() == static_cast<std::size_t>(std::distance(std::begin(gBigStruct), std::end(gBigStruct))) );

    const auto count = gBigStruct << COUNT( val, val.fValue > 500.0f );
    auto nativeCount = 0;
    for ( auto&& bigStruct : gBigStruct )
    {
        if ( bigStruct.fValue > 500.0f )
            nativeCount++;
    }
    ASSERT_EQ( nativeCount, count );
}

#if ENABLE_PERF
static void NativeCount( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto nativeCount = 0;
		for (auto&& bigStruct : gBigStruct)
		{
			if (bigStruct.fValue > 500.0f)
				nativeCount++;
		}
		MEM_COUNTER( state );
	}
}

static void LinqForCppCount( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		const auto count = gBigStruct << COUNT(val, val.fValue > 500.0f);
		MEM_COUNTER( state );
	}
}

static void CppLinqCount( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result = cpplinq::from(gBigStruct) >> cpplinq::count([&](const auto& val) {return val.fValue > 500.0f; });
		MEM_COUNTER( state );
	}
}
BENCHMARK( NativeCount );
BENCHMARK( LinqForCppCount );
BENCHMARK( CppLinqCount );
#endif
