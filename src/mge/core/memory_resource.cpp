// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory_resource.hpp"

MGE_USE_LOG(MEMORY);

namespace mge {

    tracing_memory_resource::tracing_memory_resource(
        std::pmr::memory_resource *base)
        : m_base(base)
    {}

    void *tracing_memory_resource::do_allocate(size_t bytes, size_t align)
    {
        try {
            void *result = m_base->allocate(bytes, align);
            MGE_DEBUG_LOG(MEMORY) << "do_allocate(" << bytes << ", " << align
                                  << ") => " << result;
            return result;
        } catch (const std::exception &ex) {
            MGE_DEBUG_LOG(MEMORY) << "do_allocate(" << bytes << ", " << align
                                  << ") throws " << ex.what();
            throw;
        }
    }

    void tracing_memory_resource::do_deallocate(void *ptr, size_t bytes,
                                                size_t align)
    {
        try {
            m_base->deallocate(ptr, bytes, align);
            MGE_DEBUG_LOG(MEMORY) << "do_deallocate(" << ptr << ", " << bytes
                                  << ", " << align << ")";
            return;
        } catch (const std::exception &ex) {
            MGE_DEBUG_LOG(MEMORY) << "do_deallocate(" << ptr << ", " << bytes
                                  << ", " << align << ") throws " << ex.what();
            throw;
        }
    }

    bool tracing_memory_resource::do_is_equal(
        const memory_resource &other) const noexcept
    {
        if (this == &other || &other == m_base) {
            return true;
        } else if (m_base->is_equal(other)) {
            return true;
        } else {
            try {
                const tracing_memory_resource *po =
                    dynamic_cast<const tracing_memory_resource *>(&other);
                return m_base->is_equal(*po);
            } catch (const std::exception &) {
                return false;
            }
        }
    }

    callback_memory_resource::callback_memory_resource(
        std::pmr::memory_resource *base)
        : m_base(base)
    {}

    void *callback_memory_resource::do_allocate(size_t bytes, size_t align)
    {
        void *ptr = m_base->allocate(bytes, align);
        try {
            if (m_allocate_callback) {
                m_allocate_callback(bytes, align, ptr);
            }
        } catch (const std::exception &) {
            m_base->deallocate(ptr, bytes, align);
            throw;
        }
        return ptr;
    }

    void callback_memory_resource::do_deallocate(void *ptr, size_t bytes,
                                                 size_t align)
    {
        try {
            if (m_deallocate_callback) {
                m_deallocate_callback(bytes, align, ptr);
            }
        } catch (std::exception &) {
            m_base->deallocate(ptr, bytes, align);
            throw;
        }
        m_base->deallocate(ptr, bytes, align);
    }

    bool callback_memory_resource::do_is_equal(
        const memory_resource &other) const noexcept
    {
        if (this == &other || &other == m_base) {
            return true;
        } else if (m_base->is_equal(other)) {
            return true;
        } else {
            try {
                const callback_memory_resource *po =
                    dynamic_cast<const callback_memory_resource *>(&other);
                return m_base->is_equal(*po);
            } catch (const std::exception &) {
                return false;
            }
        }
    }

    void callback_memory_resource::set_allocate_callback(
        callback_memory_resource::callback allocate_callback)
    {
        m_allocate_callback = allocate_callback;
    }

    void callback_memory_resource::set_deallocate_callback(
        callback_memory_resource::callback deallocate_callback)
    {
        m_deallocate_callback = deallocate_callback;
    }

} // namespace mge
