#pragma once
#include "mge/graphics/hardware_buffer.hpp"
namespace mge {


    class MGE_GRAPHICS_EXPORT index_buffer
            : public hardware_buffer
    {
    protected:
        index_buffer(render_context& context,
                     buffer_usage usage,
                     buffer_access cpu_access,
                     buffer_access gpu_access);
        index_buffer(render_context& context,
                     buffer_usage usage,
                     buffer_access cpu_access,
                     buffer_access gpu_access,
                     void *data,
                     size_t data_size);
    public:
        virtual ~index_buffer();
    };

}
