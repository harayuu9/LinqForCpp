#include "Test.h"

TEST( OrderBy, Normal )
{
    std::vector<CData> data(std::begin(gBigStruct), std::end(gBigStruct));
    std::sort(data.begin(), data.end(), [](const auto& data1, const auto& data2)
    {
        return data1.id < data2.id;
    });

    auto startItr = data.begin();
    for(auto itr = data.begin(); itr != data.end(); ++itr)
    {
        if(startItr->id != itr->id )
        {
            std::sort(startItr, itr, [](const auto& x1, const auto& x2)
            {
                return x1.name.length() > x2.name.length();
            });
            startItr = itr;
        }
    }

    const auto result = gBigStruct
        << ORDER_BY_ASCENDING(data, data.id)
        << THEN_BY_DESCENDING(x, x.name.length())
        << linq::ToVector();

    ASSERT_EQ(data, result );
}

#if ENABLE_PERF
static void NativeOrderBy( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        std::vector<CData> data(std::begin(gBigStruct), std::end(gBigStruct));
        std::sort(data.begin(), data.end(), [](const auto& data1, const auto& data2)
        {
            return data1.id < data2.id;
        });

        auto startItr = data.begin();
        for (auto itr = data.begin(); itr != data.end(); ++itr)
        {
            if (startItr->id != itr->id)
            {
                std::sort(startItr, itr, [](const auto& x1, const auto& x2)
                {
                    return x1.name.length() > x2.name.length();
                });
                startItr = itr;
            }
        }
        MEM_COUNTER( state );
    }
}

static void LinqForCppOrderBy( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto result = gBigStruct
            << ORDER_BY_ASCENDING(data, data.id)
            << THEN_BY_DESCENDING(x, x.name.length())
            << linq::ToVector();
        MEM_COUNTER( state );
    }
}

static void CppLinqOrderBy( benchmark::State& state )
{
    using namespace cpplinq;
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = from( gBigStruct )
            >> orderby_ascending( []( const auto& x ) { return x.id; } )
            >> thenby_descending( []( const auto& x ) { return x.name.length(); } )
            >> to_vector();
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeOrderBy );
BENCHMARK( LinqForCppOrderBy );
BENCHMARK( CppLinqOrderBy );
#endif
