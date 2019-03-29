#include "mge/graphics/hardware_buffer.hpp"

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
    {}

    hardware_buffer::~hardware_buffer()
    {}

}
