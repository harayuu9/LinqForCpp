#pragma once

namespace linq {

template<typename T>
class EmptyBuilder
{
public:
    [[nodiscard]] auto begin() const noexcept
    {
        return reinterpret_cast<T*>( 0 );
    }

    [[nodiscard]] auto end() const noexcept
    {
        return begin();
    }
};

template<typename T>
auto Empty() noexcept
{
    return EmptyBuilder<T>();
}
}
