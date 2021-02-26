#pragma once

namespace linq {

template<typename T>
class SingletonBuilder
{
public:
    explicit SingletonBuilder( const T& value ) noexcept : mValue( value )
    {
    }

    [[nodiscard]] auto begin() const noexcept
    {
        return &mValue;
    }

    [[nodiscard]] auto end() const noexcept
    {
        return begin() + 1;
    }

private:
    T mValue;
};

template<typename T>
auto Singleton( const T& value ) noexcept
{
    return SingletonBuilder<T>( value );
}
}
