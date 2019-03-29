#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/buffer_access.hpp"
#include "mge/graphics/buffer_usage.hpp"
#include "mge/graphics/buffer_type.hpp"

namespace mge {
    /**
     * Base class for hardware buffers. Hardware buffers are used for
     * indices and vertex data.
     */
    class MGE_GRAPHICS_EXPORT hardware_buffer : public context_object
    {
    protected:
        hardware_buffer(render_context& context,
                        buffer_type type,
                        buffer_usage usage,
                        buffer_access cpu_access,
                        buffer_access gpu_access);
    public:
        virtual ~hardware_buffer();
    protected:
        buffer_type  m_type;
        buffer_usage m_usage;
        buffer_access m_cpu_access;
        buffer_access m_gpu_access;
    };
}
