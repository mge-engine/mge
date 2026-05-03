// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/index_buffer_handle.hpp"
#include "mge/graphics/program_handle.hpp"
#include "mge/graphics/vertex_buffer_handle.hpp"

#include <cstddef>
#include <cstdint>

namespace mge {

    class command_buffer;
    class mesh;
    class render_context;
    class vertex_layout;

    /**
     * @brief Mesh data prepared for rendering.
     *
     * A render mesh is the GPU-side counterpart of @c mesh: it combines a
     * vertex buffer and an index buffer that can be submitted to a command
     * buffer.
     */
    class MGEGRAPHICS_EXPORT render_mesh
    {
    public:
        render_mesh() = default;

        render_mesh(const vertex_buffer_handle& vertices,
                    const index_buffer_handle&  indices);

        render_mesh(render_context& context, const mesh& source);

        const vertex_buffer_handle& vertices() const noexcept
        {
            return m_vertices;
        }

        const index_buffer_handle& indices() const noexcept
        {
            return m_indices;
        }

        const vertex_layout& layout() const;

        data_type index_element_type() const;

        size_t vertex_count() const;

        size_t index_count() const;

        explicit operator bool() const noexcept
        {
            return m_vertices && m_indices;
        }

        void draw(command_buffer&       command_buffer,
                  const program_handle& program,
                  uint32_t              index_count = 0,
                  uint32_t              index_offset = 0) const;

    private:
        vertex_buffer_handle m_vertices;
        index_buffer_handle  m_indices;
    };

} // namespace mge
