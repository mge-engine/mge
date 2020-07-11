// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/log.hpp"
#include <functional>
#include <memory_resource>

namespace mge {

    /**
     * A tracing memory resource that traces all its allocations.
     */
    class MGE_CORE_EXPORT tracing_memory_resource
        : public std::pmr::memory_resource
    {
    public:
        tracing_memory_resource(std::pmr::memory_resource *base);
        ~tracing_memory_resource() = default;

    private:
        void *do_allocate(size_t bytes, size_t align) override;
        void  do_deallocate(void *ptr, size_t bytes, size_t align) override;
        bool  do_is_equal(const memory_resource &other) const noexcept;

        std::pmr::memory_resource *m_base;
    };
    /**
     * @brief A callback memory resource that allows to install callbacks on
     * allocation and de-allocation.
     *
     */
    class MGE_CORE_EXPORT callback_memory_resource
        : public std::pmr::memory_resource
    {
    public:
        /**
         * @brief Callback function type.
         */
        using callback =
            std::function<void(size_t bytes, size_t align, void *ptr)>;
        callback_memory_resource(std::pmr::memory_resource *base);
        ~callback_memory_resource() = default;

        /**
         * @brief Set the allocate callback.
         *
         * The function is not thread safe.
         *
         * @param allocate_callback new allocate callback or @c nullptr to clear
         */
        void set_allocate_callback(callback allocate_callback);

        /**
         * @brief Set the deallocate callback.
         *
         * The function is not thread safe.
         *
         * @param deallocate_callback new deallocate callback or @c nullptr to
         * clear
         */
        void set_deallocate_callback(callback deallocate_callback);

    private:
        void *do_allocate(size_t bytes, size_t align) override;
        void  do_deallocate(void *ptr, size_t bytes, size_t align) override;
        bool  do_is_equal(const memory_resource &other) const noexcept;

        std::pmr::memory_resource *m_base;
        callback                   m_allocate_callback;
        callback                   m_deallocate_callback;
    };

} // namespace mge
