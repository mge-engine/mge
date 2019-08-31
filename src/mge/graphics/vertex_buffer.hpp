// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/hardware_buffer.hpp"
#include "mge/graphics/vertex_layout.hpp"
namespace mge {

    /**
     * Vertex buffer. 
     * The content of a vertex buffer is defined by a layout.
     */
    class MGE_GRAPHICS_EXPORT vertex_buffer
        : public hardware_buffer
    {
    protected:
        vertex_buffer(render_context& context,
                      const vertex_layout& layout,
                      buffer_change_policy change_policy,
                      buffer_access cpu_access,
                      buffer_access gpu_access,
                      size_t element_count,
                      void *initial_data = nullptr);
    public:
        virtual ~vertex_buffer();
        const vertex_layout& layout() const { return m_layout; }
        size_t element_count() const;
    protected:
        vertex_layout m_layout;

    };
}
