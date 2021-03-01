#include "Test.h"

TEST(Select, Select)
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

TEST(Select, SelectMany)
{
    std::vector<int> baseResult;
    for (auto&& data : gBigStruct)
    {
        std::vector<int> temp;
        temp.reserve(data.id);
        for (auto i = 0; i < data.id; i++)
        {
            temp.push_back(data.id * i);
        }
        baseResult.insert(baseResult.end(), temp.begin(), temp.end());
    }

    auto result = gBigStruct
        << linq::SelectMany([&](const auto& x)
    {
        std::vector<int> result;
        result.reserve(x.id);
        for (auto i = 0; i < x.id; i++)
        {
            result.push_back(x.id * i);
        }
        return result;
    })
        << linq::ToVector();
    ASSERT_EQ(result, baseResult);
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
static void LinqForCppSelect(benchmark::State& state)
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

static void NativeSelectMany(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        MEM_RESET();
        std::vector<int> result;
        for (auto&& data : gBigStruct)
        {
            std::vector<int> temp;
            temp.reserve(data.id);
            for (auto i = 0; i < data.id; i++)
            {
                temp.push_back(data.id * i);
            }
            result.insert(result.end(), temp.begin(), temp.end());
        }
        MEM_COUNTER(state);
    }
}

static void LinqForCppSelectMany(benchmark::State& state)
{
    using namespace linq;

    while (state.KeepRunning())
    {
        MEM_RESET();
        auto result = gBigStruct
            << SelectMany([&](const auto& x)
        {
            std::vector<int> result;
            result.reserve(x.id);
            for (auto i = 0; i < x.id; i++)
            {
                result.push_back(x.id * i);
            }
            return result;
        })
            << ToVector();
        MEM_COUNTER(state);
    }
}

// library bug
//static void CppLinqSelectMany(benchmark::State& state)
//{
//    while (state.KeepRunning())
//    {
//        MEM_RESET();
//        auto result = cpplinq::from(gBigStruct)
//            >> cpplinq::select_many([&](const auto& x)
//        {
//            std::vector<int> is;
//            is.reserve(x.id);
//            for (auto i = 0; i < x.id; i++)
//            {
//                is.push_back(x.id * i);
//            }
//            return cpplinq::from_copy(is);
//        })
//            >> cpplinq::to_vector();
//        MEM_COUNTER(state);
//    }
//}

BENCHMARK(NativeSelect);
BENCHMARK(LinqForCppSelect);
BENCHMARK(CppLinqSelect);
BENCHMARK(NativeSelectMany);
BENCHMARK(LinqForCppSelectMany);

// library bug
//BENCHMARK(CppLinqSelectMany);
#endif