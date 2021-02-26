#include "Test.h"

std::vector<CData> gBigStruct;
void Initialize()
{
    gBigStruct.reserve(10000);
    for (auto i = 0; i < 10000;i++)
    {
        gBigStruct.push_back(
            {
                rand() % 5000,
                std::to_string(rand() % 30000),
                (rand() % 1'500'000) / 1000.0f ,
                static_cast<std::size_t>(rand() % 40000)
            });
    }
}

#if ENABLE_PERF
int main( int argc, char** argv )
{
    Initialize();
    benchmark::Initialize( &argc, argv );
    if ( benchmark::ReportUnrecognizedArguments( argc, argv ) ) return 1;
    benchmark::RunSpecifiedBenchmarks();
}
#else
GTEST_API_ int main(int argc, char** argv) {
    Initialize();
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
