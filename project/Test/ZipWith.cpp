#include "Test.h"

TEST( ZipWith, Normal )
{
    std::vector<std::pair<CData, CData>> data;
    data.reserve( std::size( gBigStruct ) );

    for ( auto i = 0; i < std::size( gBigStruct ); i++ )
    {
        data.emplace_back( gBigStruct[i], gBigStruct[i] );
    }

    const auto result = gBigStruct
        << linq::ZipWith( gBigStruct )
        << linq::ToVector();

    ASSERT_EQ( data, result );
}

#if ENABLE_PERF
static void NativeZipWith( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        std::vector<std::pair<CData, CData>> data;
        data.reserve( std::size( gBigStruct ) );

        for ( auto i = 0; i < std::size( gBigStruct ); i++ )
        {
            data.emplace_back( gBigStruct[i], gBigStruct[i] );
        }
        MEM_COUNTER( state );
    }
}

static void LinqForCppZipWith( benchmark::State& state )
{
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        const auto result = gBigStruct
            << linq::ZipWith( gBigStruct )
            << linq::ToVector();
        MEM_COUNTER( state );
    }
}

static void CppLinqZipWith( benchmark::State& state )
{
    using namespace cpplinq;
    while ( state.KeepRunning() )
    {
        MEM_RESET();
        auto result = from( gBigStruct )
            >> zip_with( from( gBigStruct ) )
            >> to_vector();
        MEM_COUNTER( state );
    }
}

BENCHMARK( NativeZipWith );
BENCHMARK( LinqForCppZipWith );
BENCHMARK( CppLinqZipWith );
#endif
