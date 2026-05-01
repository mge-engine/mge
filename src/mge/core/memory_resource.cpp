// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory_resource.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/statistics.hpp"

using namespace std::string_view_literals;

namespace mge {

    class memory_statistics : public statistics
    {
    public:
        memory_statistics()
            : statistics(statistics::root(), "memory"sv)
        {
            allocations = 0;
            deallocations = 0;
            allocated_bytes = 0;
        }

        ~memory_statistics() override
        {
            release();
        }

        const description& describe() const override
        {
            static statistics::description desc(
                "memory"sv,
                "Memory allocation statistics"sv,
                {statistics::description::field(
                     "allocations"sv,
                     &memory_statistics::allocations),
                 statistics::description::field(
                     "deallocations"sv,
                     &memory_statistics::deallocations),
                 statistics::description::field(
                     "allocated_bytes"sv,
                     &memory_statistics::allocated_bytes)});
            return desc;
        }

        statistics::counter_type allocations;
        statistics::counter_type deallocations;
        statistics::counter_type allocated_bytes;
    };

    static memory_statistics s_memory_statistics;

    class named_memory_statistics : public statistics
    {
    public:
        named_memory_statistics(std::string_view name)
            : statistics(s_memory_statistics, name)
        {
            allocations = 0;
            deallocations = 0;
            allocated_bytes = 0;
        }

        ~named_memory_statistics() override = default;

        const description& describe() const override
        {
            static statistics::description desc(
                "named_memory"sv,
                "Named memory resource statistics"sv,
                {statistics::description::field(
                     "allocations"sv,
                     &named_memory_statistics::allocations),
                 statistics::description::field(
                     "deallocations"sv,
                     &named_memory_statistics::deallocations),
                 statistics::description::field(
                     "allocated_bytes"sv,
                     &named_memory_statistics::allocated_bytes)});
            return desc;
        }

        statistics::counter_type allocations;
        statistics::counter_type deallocations;
        statistics::counter_type allocated_bytes;
    };

    class global_memory_resource : public memory_resource
    {
    public:
        global_memory_resource()
            : memory_resource()
        {}

        ~global_memory_resource() override = default;

        void* do_allocate(size_t bytes, size_t alignment) override
        {
            ++s_memory_statistics.allocations;
            s_memory_statistics.allocated_bytes += bytes;
            return mge::allocate(bytes, alignment);
        }

        void do_deallocate(void* p, size_t bytes, size_t alignment) override
        {
            ++s_memory_statistics.deallocations;
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
        , m_statistics(new named_memory_statistics(
              std::string_view(m_name.data(), m_name.size())))
    {}

    named_memory_resource::~named_memory_resource()
    {
        m_statistics->release();
        delete m_statistics;
    }

    void* named_memory_resource::do_allocate(size_t bytes, size_t alignment)
    {
        ++m_statistics->allocations;
        m_statistics->allocated_bytes += bytes;
        return m_upstream->allocate(bytes, alignment);
    }

    void named_memory_resource::do_deallocate(void*  p,
                                              size_t bytes,
                                              size_t alignment)
    {
        ++m_statistics->deallocations;
        m_upstream->deallocate(p, bytes, alignment);
    }

    const statistics&
    named_memory_resource::resource_statistics() const noexcept
    {
        return *m_statistics;
    }

    statistics& named_memory_resource::resource_statistics() noexcept
    {
        return *m_statistics;
    }
} // namespace mge