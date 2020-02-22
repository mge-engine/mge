#include "mge/core/memory_resource.hpp"

MGE_USE_LOG(MGE);

namespace mge {
    void *
    tracing_memory_resource::do_allocate(size_t bytes, size_t align)
    {
        try {
            void *result = m_base->allocate(bytes, align);
            MGE_DEBUG_LOG(MGE) << "do_allocate(" << bytes << ", " << align << ") => " << result;
            return result;
        } catch(const std::exception& ex) {
            MGE_DEBUG_LOG(MGE) << "do_allocate(" << bytes << ", " << align << ") throws " << ex.what();
            throw;
        }
    }

    void
    tracing_memory_resource::do_deallocate(void *ptr, size_t bytes, size_t align)
    {
        try {
            m_base->deallocate(ptr, bytes, align);
            MGE_DEBUG_LOG(MGE) << "do_deallocate(" << ptr << ", " << bytes << ", " << align << ")";
            return;
        } catch(const std::exception& ex) {
            MGE_DEBUG_LOG(MGE) << "do_deallocate(" << ptr << ", " << bytes << ", " << align << ") throws " << ex.what();
            throw;
        }
    }

    bool
    tracing_memory_resource::do_is_equal(const memory_resource &other) const noexcept
    {
        if(this == &other || &other == m_base) {
            return true;
        } else if(m_base->is_equal(other)) {
            return true;
        } else {
            try {
                const tracing_memory_resource *po = dynamic_cast<const tracing_memory_resource *>(&other);
                return m_base->is_equal(*po);
            } catch(const std::exception&) {
                return false;
            }
        }
    }



}
