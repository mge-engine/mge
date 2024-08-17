// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory_resource.hpp"
#include "mge/core/memory.hpp"

namespace mge {

    void* memory_resource::do_allocate(size_t bytes, size_t alignment)
    {
        return mge::allocate(bytes, alignment);
    }

    void memory_resource::do_deallocate(void* p, size_t bytes, size_t alignment)
    {
        mge::deallocate(p, bytes, alignment);
    }

    bool memory_resource::do_is_equal(
        const std::pmr::memory_resource& other) const noexcept
    {
        return this == &other;
    }

    ::mge::memory_resource memory_resource::instance;

    static auto* get_memory_resource_instance()
    {
        (void)memory_resource::instance.is_equal(memory_resource::instance);
        return &memory_resource::instance;
    }

    std::pmr::polymorphic_allocator<void> memory_resource::allocator{
        get_memory_resource_instance()};
} // namespace mge