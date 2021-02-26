#include "Test.h"

TEST( Where, Normal )
{
	std::vector<CData> baseResult;
	for ( auto&& bigStruct : gBigStruct )
	{
		if ( bigStruct.id > 100 )
			baseResult.push_back( bigStruct );
	}

	const auto result = gBigStruct << WHERE( val, val.id > 100 ) << linq::ToVector();
	ASSERT_EQ( baseResult, result );
}

#if ENABLE_PERF
static void NativeWhere( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		std::vector<CData> baseResult;
		for ( auto&& bigStruct : gBigStruct )
		{
			if ( bigStruct.id > 100 )
				baseResult.push_back( bigStruct );
		}
		MEM_COUNTER( state );
	}
}

static void MyLinqWhere( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result = gBigStruct << WHERE( val, val.id > 100 ) << linq::ToVector();
		MEM_COUNTER( state );
	}
}

static void CppLinqWhere( benchmark::State& state )
{
	while ( state.KeepRunning() )
	{
		MEM_RESET();
		auto result =
			cpplinq::from( gBigStruct ) >>
			cpplinq::where( []( const auto& val ) { return val.id > 100; } ) >>
			cpplinq::to_vector();
		MEM_COUNTER( state );
	}
}

BENCHMARK( NativeWhere );
BENCHMARK( MyLinqWhere );
BENCHMARK( CppLinqWhere );
#endif
