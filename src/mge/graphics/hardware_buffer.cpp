#include "mge/graphics/hardware_buffer.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stacktrace.hpp"

MGE_USE_IMPORTED_LOG(MGE);

namespace mge {

    hardware_buffer::hardware_buffer(render_context &context,
                                     buffer_type type,
                                     buffer_usage usage,
                                     buffer_access cpu_access,
                                     buffer_access gpu_access)
        :context_object(context)
        ,m_type(type)
        ,m_usage(usage)
        ,m_cpu_access(cpu_access)
        ,m_gpu_access(gpu_access)
        ,m_map_count(0)
        ,m_mapped_memory(nullptr)
    {}

    hardware_buffer::~hardware_buffer()
    {
        if(mapped()) {
            MGE_WARNING_LOG(MGE) << "Destroying mapped buffer ";
            MGE_WARNING_LOG(MGE) << mge::stacktrace();
        }
    }

    void *
    hardware_buffer::map()
    {
        if(!m_map_count) {
            m_mapped_memory = on_map();
        }
        ++m_map_count;
        return m_mapped_memory;
    }

    void
    hardware_buffer::unmap()
    {
        if(m_map_count == 1) {
            on_unmap();
            m_map_count = 0;
            m_mapped_memory = nullptr;
        } else if(m_map_count > 0) {
            --m_map_count;
        }
    }

}
