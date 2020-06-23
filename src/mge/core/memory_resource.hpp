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

    class MGE_CORE_EXPORT callback_memory_resource
        : public std::pmr::memory_resource
    {
    public:
        using callback =
            std::function<void(size_t bytes, size_t align, void *ptr)>;
        callback_memory_resource(std::pmr::memory_resource *base);
        ~callback_memory_resource() = default;

        void set_allocate_callback(callback allocate_callback);
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
