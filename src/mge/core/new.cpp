// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/new.hpp"
#include "mge/core/memory.hpp"
#include <atomic>
#include <stdexcept>

namespace mge {

    static std::atomic<uint64_t> s_global_allocation_count;
    static thread_local uint64_t s_thread_allocation_count;

    uint64_t allocation_count() { return s_global_allocation_count.load(); }

    uint64_t thread_allocation_count() { return s_thread_allocation_count; }

    void *operator_new(std::size_t count)
    {
        ++s_global_allocation_count;
        ++s_thread_allocation_count;
        void *ptr = mge::malloc(count);
        if (ptr) {
            return ptr;
        }
        throw std::bad_alloc();
    }

    void *operator_new_array(std::size_t count)
    {
        ++s_global_allocation_count;
        ++s_thread_allocation_count;
        void *ptr = mge::malloc(count);
        if (ptr) {
            return ptr;
        }
        throw std::bad_alloc();
    }

    void *operator_new_noexcept(std::size_t count) noexcept
    {
        ++s_global_allocation_count;
        ++s_thread_allocation_count;
        void *ptr = mge::malloc(count);
        return ptr;
    }

    void *operator_new_array_noexcept(std::size_t count) noexcept
    {
        ++s_global_allocation_count;
        ++s_thread_allocation_count;
        void *ptr = mge::malloc(count);
        return ptr;
    }

    void operator_delete(void *ptr)
    {
        if (ptr)
            mge::free(ptr);
    }
    void operator_delete_array(void *ptr)
    {
        if (ptr)
            mge::free(ptr);
    }
    void operator_delete_noexcept(void *ptr) noexcept
    {
        if (ptr)
            mge::free(ptr);
    }
    void operator_delete_array_noexcept(void *ptr) noexcept
    {
        if (ptr)
            mge::free(ptr);
    }

} // namespace mge
