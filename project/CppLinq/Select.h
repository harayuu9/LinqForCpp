#pragma once
#include "IteratorBase.h"
#include "TypeTraits.h"

#define SELECT(variant, convert) ::linq::Select([&](const auto& (variant)){ return convert; })
namespace linq {
/**
 * \brief Select
 * \tparam Func Select Function
 */
template<typename Func>
class SelectBuilder
{
    /**
     * \brief Can call Select
     *         Function result must be not void
     * \tparam T Have iterator
     */
    template<class T>
    class is_called_select
    {
        template<class U> static auto test( int ) -> decltype(std::declval<Func>()( *std::begin( std::declval<U>() ) ));
        template<class U> static void test( ... );
        template<class U> static auto constTest( int ) -> decltype(std::declval<Func>()( *std::begin( std::declval<const U>() ) ));
        template<class U> static void constTest( ... );
    public:
        static constexpr bool value = !std::is_void_v<decltype(test<T>( 0 ))> || !std::is_void_v<decltype(constTest<T>( 0 ))>;
    };

    /**
     * \brief Impl have iterator
     * \tparam Array ArrayType
     */
    template<class Array>
    class Impl
    {
        using RawItr = decltype(Begin(std::declval<Array>()));
    public:
        Impl( Impl&& other ) = default;
        Impl& operator=( Impl&& other ) = default;

        Impl( Array&& arr, Func&& func ) : mArray( std::move( arr ) ), mFunc( std::move( func ) )
        {
        }

        struct Iterator : IteratorBase<RawItr>
        {
            using value_type = decltype(std::declval<Func>()( *std::declval<RawItr>() ));
            using pointer = std::add_pointer_t<value_type>;
            using reference = std::add_lvalue_reference_t<value_type>;

            Iterator() = default;
            explicit Iterator( RawItr itr, const Func* pFunc ) noexcept : mpFunc( pFunc )
            {
                IteratorBase<RawItr>::mRawIterator = std::move( itr );
            }

            auto operator->() const noexcept
            {
                return ( *mpFunc )( *IteratorBase<RawItr>::mRawIterator );
            }

            auto operator*() const noexcept
            {
                return ( *mpFunc )( *IteratorBase<RawItr>::mRawIterator );
            }

        private:
            const Func* mpFunc = nullptr;
        };

        [[nodiscard]] auto begin() const noexcept
        {
            auto beginItr = Begin( mArray );
            return Iterator( beginItr, &mFunc );
        }

        [[nodiscard]] auto end() const noexcept
        {
            auto endItr = End( mArray );
            return Iterator( endItr, &mFunc );
        }

        /**
         * \brief For is_select(SFINAE).
         */
        static void select();
    private:
        Array mArray;
        Func mFunc;
    };

public:
    explicit SelectBuilder( Func&& func ) noexcept : mFunc( std::move( func ) )
    {
    }

    template<class Array>
    auto build( Array&& arr ) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");
        static_assert(is_called_select<Array>::value, "function result is void or don't call function");

        if constexpr ( std::is_lvalue_reference_v<Array> )
        {
            return Impl<decltype(&arr)>( &arr, std::move( mFunc ) );
        }
        else
        {
            return Impl<Array>( std::forward<Array>( arr ), std::move( mFunc ) );
        }
    }

private:
    Func mFunc;
};
template<class Array>
class is_select
{
    template<typename U> static auto test(int) -> decltype(U::select(), std::true_type());
    template<typename U> static auto test(...)->std::false_type;
public:
    using Type = decltype(test<Array>(1));
};
template<class Array>
constexpr bool is_select_v = is_select<std::remove_reference_t<Array>>::Type::value;

template<typename Func>
SelectBuilder<Func> Select( Func&& func ) noexcept
{
    return SelectBuilder<Func>( std::forward<Func>( func ) );
}

/**
 * \brief SelectMany
 * \tparam Func SelectMany Function
 */
template<typename Func>
class SelectManyBuilder
{
    /**
     * \brief Can call SelectMany
     *         Function result must be not void
     * \tparam T Have iterator
     */
    template<class T>
    class is_called_select_many
    {
        template<class U> static auto test(int) -> decltype(std::declval<Func>()(*std::begin(std::declval<U>())));
        template<class U> static void test(...);
        template<class U> static auto constTest(int) -> decltype(std::declval<Func>()(*std::begin(std::declval<const U>())));
        template<class U> static void constTest(...);
    public:
        using Type = std::conditional_t<std::is_void_v<decltype(test<T>(0))>, decltype(constTest<T>(0)), decltype(test<T>(0)) >;
        static constexpr bool value = !std::is_void_v<Type>;
    };

    /**
     * \brief Impl have iterator
     * \tparam Array ArrayType
     */
    template<class Array>
    class Impl
    {
        using RawItr = decltype(Begin(std::declval<Array>()));
    public:
        Impl(Impl&& other) = default;
        Impl& operator=(Impl&& other) = default;

        Impl(Array&& arr, Func&& func) : mArray(std::move(arr)), mFunc(std::move(func))
        {
        }

        struct Iterator : IteratorBase<RawItr>
        {
            using FuncResultArray = decltype(std::declval<Func>()(*std::declval<RawItr>()));
            using FuncResultIterator = decltype(std::begin(std::declval<FuncResultArray>()));
            using iterator_category = std::forward_iterator_tag;
            using value_type = array_type_t<FuncResultArray>;
            using pointer = std::add_pointer_t<value_type>;
            using reference = std::add_lvalue_reference_t<value_type>;

            Iterator() = default;
            explicit Iterator(RawItr itr, RawItr endItr, const Func* pFunc) noexcept : mpFunc(pFunc)
            {
                IteratorBase<RawItr>::mRawIterator = std::move(itr);
                mRawEnd = std::move(endItr);
                if (IteratorBase<RawItr>::mRawIterator != mRawEnd)
                {
                    mArray = (*mpFunc)(*IteratorBase<RawItr>::mRawIterator);
                    mCurrentIterator = std::begin(mArray);
                    mEndIterator = std::end(mArray);
                }
            }

            Iterator& operator++() noexcept
            {
                ++mCurrentIterator;
                if(mCurrentIterator == mEndIterator)
                {
                    ++IteratorBase<RawItr>::mRawIterator;
                    if (IteratorBase<RawItr>::mRawIterator != mRawEnd)
                    {
                        mArray = (*mpFunc)(*IteratorBase<RawItr>::mRawIterator);
                        mCurrentIterator = std::begin(mArray);
                        mEndIterator = std::end(mArray);
                    }
                }
                return *this;
            }
            Iterator operator++(int) noexcept
            {
                ++mCurrentIterator;
                if (mCurrentIterator == mEndIterator)
                {
                    ++IteratorBase<RawItr>::mRawIterator;
                    if (IteratorBase<RawItr>::mRawIterator != mRawEnd)
                    {
                        mArray = (*mpFunc)(*IteratorBase<RawItr>::mRawIterator);
                        mCurrentIterator = std::begin(mArray);
                        mEndIterator = std::end(mArray);
                    }
                }
                return *this;
            }
            auto operator->() const noexcept
            {
                return *mCurrentIterator;
            }

            auto operator*() const noexcept
            {
                return *mCurrentIterator;
            }

        private:
            const Func* mpFunc = nullptr;
            RawItr mRawEnd;
            FuncResultArray mArray;
            FuncResultIterator mCurrentIterator, mEndIterator;
        };

        [[nodiscard]] auto begin() const noexcept
        {
            auto beginItr = Begin(mArray);
            auto endItr = End(mArray);
            return Iterator(beginItr, endItr, &mFunc);
        }

        [[nodiscard]] auto end() const noexcept
        {
            auto endItr = End(mArray);
            return Iterator(endItr, endItr, &mFunc);
        }

        /**
         * \brief For is_select_many(SFINAE).
         */
        static void selectMany();
        friend struct ToVectorBuilder;
        friend struct ToListBuilder;
    private:
        Array mArray;
        Func mFunc;
    };

public:
    explicit SelectManyBuilder(Func&& func) noexcept : mFunc(std::move(func))
    {
    }

    template<class Array>
    auto build(Array&& arr) noexcept
    {
        static_assert(have_const_iterator_v<Array>, "T have not iterator");
        static_assert(is_called_select_many<Array>::value, "function result is void or don't call function");
        static_assert(have_iterator_v<typename is_called_select_many<Array>::Type> ||
            have_const_iterator_v<typename is_called_select_many<Array>::Type>, "function result don't have iterator or don't call function");
        
        if constexpr (std::is_lvalue_reference_v<Array>)
        {
            return Impl<decltype(&arr)>(&arr, std::move(mFunc));
        }
        else
        {
            return Impl<Array>(std::forward<Array>(arr), std::move(mFunc));
        }
    }

private:
    Func mFunc;
};
template<class Array>
class is_select_many
{
    template<typename U> static auto test(int) -> decltype(U::selectMany(), std::true_type());
    template<typename U> static auto test(...)->std::false_type;
public:
    using Type = decltype(test<Array>(1));
};
template<class Array>
constexpr bool is_select_many_v = is_select_many< std::remove_reference_t<Array>>::Type::value;

template<typename Func>
SelectManyBuilder<Func> SelectMany(Func&& func) noexcept
{
    return SelectManyBuilder<Func>(std::forward<Func>(func));
}
}
