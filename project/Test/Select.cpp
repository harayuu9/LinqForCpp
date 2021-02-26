#include "Test.h"

TEST(Select, Normal)
{
    std::vector<std::string> baseResult;
    baseResult.reserve(std::size(gBigStruct));
    for (auto&& bigStruct : gBigStruct)
    {
        baseResult.push_back(bigStruct.name);
    }

    const auto result = gBigStruct << SELECT(val, val.name) << linq::ToVector();
    ASSERT_EQ(baseResult, result);
}

#if ENABLE_PERF
static void NativeSelect(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        MEM_RESET();
        std::vector<std::string> result;
        result.reserve(std::size(gBigStruct));
        for (auto&& bigStruct : gBigStruct)
        {
            result.push_back(bigStruct.name);
        }
        MEM_COUNTER(state);
    }
}
static void MyLinqSelect(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        MEM_RESET();
        auto result = gBigStruct << SELECT(val, val.name) << linq::ToVector();
        MEM_COUNTER(state);
    }
}
static void CppLinqSelect(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        MEM_RESET();
        auto result =
            cpplinq::from(gBigStruct) >> cpplinq::select([](const auto& val) {return val.name; }) >> cpplinq::to_vector();
        MEM_COUNTER(state);
    }
}

BENCHMARK(NativeSelect);
BENCHMARK(MyLinqSelect);
BENCHMARK(CppLinqSelect);
#endif