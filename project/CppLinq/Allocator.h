#pragma once
#include <cstdlib>

namespace linq {

// custom Allocator
//template<class T>
//struct Allocator
//{
//    using value_type = T;
//
//    Allocator() {}
//
//    template <class U>
//    Allocator(const Allocator<U>&) {}
//
//    T* allocate(const std::size_t n)
//    {
//        return static_cast<T*>(::operator new(sizeof(T)*n));
//    }
//
//    void deallocate(T* p, const std::size_t n)
//    {
//        ::operator delete(p, n);
//    }
//
//    template <class U>
//    bool operator==( const Allocator<U>&) const
//    {
//        return true;
//    }
//
//    template <class U>
//    bool operator!=(const Allocator<U>&) const
//    {
//        return false;
//    }
//};

template<class T>
using Allocator = std::allocator<T>;}