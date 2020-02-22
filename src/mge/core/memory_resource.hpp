#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/log.hpp"
#include <memory_resource>

namespace mge {

    class MGE_CORE_EXPORT tracing_memory_resource
            : public std::pmr::memory_resource
    {
    public:
        tracing_memory_resource(std::pmr::memory_resource *base);
        ~tracing_memory_resource() = default;

    private:
        void *do_allocate(size_t bytes, size_t align) override;
        void do_deallocate(void *ptr, size_t bytes, size_t align) override;
        bool do_is_equal(const memory_resource &other) const noexcept;

        std::pmr::memory_resource *m_base;
    };

}
