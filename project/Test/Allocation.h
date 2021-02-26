#pragma once
#include <cinttypes>
#define ALLOC_BENCHMARK ENABLE_PERF

#if ALLOC_BENCHMARK
static double g_allocCnt = 0;
static double g_numAlloc = 0;

inline void* operator new(std::size_t size)
{
    g_allocCnt++;
    g_numAlloc += size;
    return std::malloc(size);
}

inline void* operator new[](std::size_t size)
{
    g_allocCnt++;
    g_numAlloc += size;
    return std::malloc(size);
}

inline void operator delete(void* ptr)
{
    std::free(ptr);
}
inline void operator delete(void* ptr, std::size_t)
{
    std::free(ptr);
}

inline void operator delete[](void* ptr)
{
    std::free(ptr);
}
inline void operator delete[](void* ptr, std::size_t)
{
    std::free(ptr);
}

inline void memReset()
{
    g_allocCnt = 0;
    g_numAlloc = 0;
}

#define MEM_RESET() memReset()
#define MEM_COUNTER(state) state.counters.insert({ {"Alloc Memory", g_numAlloc},{"Alloc Cnt", g_allocCnt} })
#else
#define MEM_RESET() (void)0
#define MEM_COUNTER(state) (void)0

#endif
