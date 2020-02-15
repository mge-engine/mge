#pragma once
#include "mge/core/type_name.hpp"
#include <type_traits>
#include <utility>
#include <algorithm>
#include <functional>
#include <optional>
#include <new>

namespace mge {

    /**
     * A tagged value.
     *
     * A normal @c typedef or @c using declaration does not
     * declare a new type, and so cannot be used in places
     * where different components should have the same type,
     * except that it should *not* be the same type.
     *
     * An example is the entity component system, where components
     * are stored and identified by type.
     */
    template <typename T, typename TAG>
    class tagged
    {
    public:
        using value_type = T;
        using pointer_type = T*;
        using const_pointer_type = const T*;
        using tag_type = T;

        static std::string tag()
        {
            return base_type_name<TAG>();
        }

    private:
        pointer_type pointer() noexcept
        {
            return reinterpret_cast<T *>(&m_storage);
        }

        const_pointer_type pointer() const noexcept
        {
            return reinterpret_cast<const T *>(&m_storage);
        }

        template <typename... ArgTypes>
        void construct(ArgTypes&&...args) noexcept(std::is_nothrow_constructible<T, ArgTypes...>::value)
        {
            void *ptr = reinterpret_cast<void *>(&m_storage);
            new (ptr) T(std::forward< ArgTypes >( args )...);
        }

        static constexpr bool noexcept_destructible
            = std::is_nothrow_destructible< T >::value;

        void destroy() noexcept(noexcept_destructible)
        {
            T* ptr = reinterpret_cast<T *>(&m_storage);
            ptr->~T();
        }
    public:
        template <typename = std::enable_if_t<std::is_constructible<T>::value > >
        tagged() noexcept(std::is_nothrow_constructible<T>::value)
        {
            construct();
        }

        template <typename... ArgTypes,
                  typename = std::enable_if_t<std::is_constructible<T, ArgTypes...>::value > >
        tagged(ArgTypes&& ... args) noexcept(std::is_nothrow_constructible<T, ArgTypes...>::value)
        {
            construct(std::forward< ArgTypes>( args )...);
        }

        template <typename U,
                  typename UT,
                  typename = std::enable_if_t<std::is_constructible<T, U>::value > >
        tagged(const tagged<U, UT>& other) noexcept(std::is_nothrow_constructible<T, U>::value)
        {
            construct(other.get());
        }

        template <typename U,
                  typename UT,
                  typename = std::enable_if_t<std::is_constructible<T, U>::value > >
        tagged(tagged<U, UT>&& other) noexcept(std::is_nothrow_constructible<T, U>::value)
        {
            construct(std::move(other.rvalue()));
        }

        ~tagged() noexcept(noexcept_destructible)
        {
            destroy();
        }

        template <typename U,
                  typename UT,
                  typename = std::enable_if_t<std::assignable<T, U>::value > >
        tagged<T, TAG>& operator=(const tagged<U, UT>& other) noexcept(std::is_nothrow_assignable<T, U>::value)
        {
            *pointer = other.get();
            return *this;
        }

        operator const T& () const noexcept
        {
            return get();
        }


        operator T&& () noexcept
        {
            return rvalue();
        }

        /**
         * Access value.
         * @return value
         */
        const value_type& get() const noexcept
        {
            return *pointer();
        }

        value_type&& rvalue() noexcept
        {
            return std::move(*pointer());
        }

    private:
        using storage_type = std::aligned_storage_t<sizeof(T), alignof(T)>;
        storage_type m_storage;
    };


}
