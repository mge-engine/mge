// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/hardware_buffer.hpp"
#include "mge/graphics/data_type.hpp"
namespace mge {


    /**
     * An index buffer. An index buffer
     */
    class MGE_GRAPHICS_EXPORT index_buffer
            : public hardware_buffer
    {
    protected:
        index_buffer(render_context& context,
                     data_type type,
                     buffer_usage usage,
                     buffer_access cpu_access,
                     buffer_access gpu_access);
        index_buffer(render_context& context,
                     data_type type,
                     buffer_usage usage,
                     buffer_access cpu_access,
                     buffer_access gpu_access,
                     void *data,
                     size_t data_size);
    public:
        virtual ~index_buffer();
    protected:
        data_type m_type;

    };

}
