// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory_resource.hpp"
#include "mge/core/memory.hpp"

namespace mge {

    class global_memory_resource : public memory_resource
    {
    public:
        global_memory_resource()
            : memory_resource()
        {}

        ~global_memory_resource() override = default;

        void* do_allocate(size_t bytes, size_t alignment) override
        {
            return mge::allocate(bytes, alignment);
        }

        void do_deallocate(void* p, size_t bytes, size_t alignment) override
        {
            mge::deallocate(p, bytes, alignment);
        }

        bool do_is_equal(
            const std::pmr::memory_resource& other) const noexcept override
        {
            return this == &other;
        }

        static global_memory_resource* instance()
        {
            static global_memory_resource instance;
            return &instance;
        }
    };

    memory_resource::memory_resource()
        : std::pmr::memory_resource()
    {}

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

    mge::memory_resource& memory_resource::instance()
    {
        return *global_memory_resource::instance();
    }

    std::pmr::polymorphic_allocator<void> memory_resource::allocator{
        global_memory_resource::instance()};

    named_memory_resource::named_memory_resource(std::string_view name,
                                                 memory_resource* upstream)
        : m_upstream(upstream ? upstream : &memory_resource::instance())
        , m_name(name, m_upstream)
    {}
} // namespace mge