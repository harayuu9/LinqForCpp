# Linq For Cpp
[![CMake Test](https://github.com/harayuu9/LinqForCpp/actions/workflows/build_test.yml/badge.svg)](https://github.com/harayuu9/LinqForCpp/actions/workflows/build_test.yml)
* I am implementing LINQ in C++ that can be used in C#.
* LinqForCpp always compares its speed with that of Native, so it has superior performance.
* Most of the processes ran at about the same speed as Native, with some lazy execution giving better performance than Native.
* [Performance Test Result](https://harayuu9.github.io/LinqForCpp/)
* Although allocation is done internally, you can easily insert your own allocator by rewriting Allocator.h.

## Supported Version
* C++17,C++20

## Getting Started
* can use a single header file or a split header file. check the [releases](https://github.com/harayuu9/LinqForCpp/releases) page, download `LinqForCpp.zip`.
*Please add `Linq` or `SingleHeader/Linq.hpp` to my project!

```cpp
#include "Linq.h"
// or
#include "Linq.hpp"

int main()
{
    // You can use a collection that can call std::begin(), std::end()
    std::vector<int> arr = { 0, 3, 6, 1, 60, 35 };

    // Call Linq().
    auto strArray = arr
        << linq::Where( []( const int val ) { return val > 10; } )
        << linq::Select( []( const int val ) { return std::to_string( val ); } );

    // Call Linq for Macro.
    auto macroStrArray = arr
        << WHERE( val, val > 10 )
        << SELECT( val, std::to_string(val) );

    // Range base for loop
    // If you don't need to store the values, 
    // it is faster than calling ToVector() etc. to make a collection.
    for ( auto&& val : strArray )
    {
        std::cout << val << " ";
    }

    // ToStlCollection
    auto str = strArray << linq::ToVector();
}
```

## Call Timing
* As with LINQ, delayed execution is used in principle.
* However, please note that the functions to be lazily executed are different from those of LINQ.
* For performance reasons, some functions are executed immediately.
* For example, Distinct() is an immediate execution.

## License
MIT
