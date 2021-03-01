#include "Test.h"

TEST( PairWise, Normal )
{
    std::vector<std::pair<CData, CData>> data;
    data.reserve(std::size(gBigStruct) - 1);

    for(auto i=0;i<std::size(gBigStruct) - 1;i++)
    {
        data.emplace_back(gBigStruct[i], gBigStruct[i + 1]);
    }

    const auto result = gBigStruct
        << linq::PairWise()
        << linq::ToVector();

    ASSERT_EQ(data, result );
}

#if ENABLE_PERF
static void NativePairWise( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        std::vector<std::pair<CData, CData>> data;
        data.reserve(std::size(gBigStruct) - 1);

        for (auto i = 0; i < std::size(gBigStruct) - 1; i++)
        {
            data.emplace_back(gBigStruct[i], gBigStruct[i + 1]);
        }
        MEM_COUNTER( state );
    }
}

static void LinqForCppPairWise( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto result = gBigStruct
            << linq::PairWise()
            << linq::ToVector();
        MEM_COUNTER( state );
    }
}

static void CppLinqPairWise( benchmark::State& state )
{
    using namespace cpplinq;
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = from( gBigStruct )
            >> pairwise()
            >> to_vector();
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativePairWise );
BENCHMARK( LinqForCppPairWise );
BENCHMARK( CppLinqPairWise );
#endif
