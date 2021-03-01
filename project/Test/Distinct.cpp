#include "Test.h"

TEST( Distinct, Normal )
{
    const auto count = gBigStruct << linq::Distinct() << linq::Count();

    const std::set<CData> set( std::begin( gBigStruct ), std::end( gBigStruct ) );

    ASSERT_EQ( count, set.size() );
}

#if ENABLE_PERF
static void NativeDistinct( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const std::set<CData> set( std::begin( gBigStruct ), std::end( gBigStruct ) );
        std::vector<CData> result( set.begin(), set.end() );
        MEM_COUNTER( state );
    }
}

static void LinqForCppDistinct( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = gBigStruct << linq::Distinct() << linq::ToVector();
        MEM_COUNTER( state );
    }
}

static void CppLinqDistinct( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = cpplinq::from( gBigStruct ) >> cpplinq::distinct() >> cpplinq::to_vector();
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeDistinct );
BENCHMARK( LinqForCppDistinct );
BENCHMARK( CppLinqDistinct );
#endif
