// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/index_buffer_handle.hpp"
#include "mge/graphics/program_handle.hpp"
#include "mge/graphics/vertex_buffer_handle.hpp"

namespace mge {

    /**
     * @brief A command buffer records rendering commands to be
     * submitted to a render context.
     */
    class MGEGRAPHICS_EXPORT command_buffer
    {
    public:
        command_buffer() = default;
        ~command_buffer() = default;

        command_buffer(const command_buffer&) = delete;
        command_buffer(command_buffer&&) noexcept = default;
        command_buffer& operator=(const command_buffer&) = delete;
        command_buffer& operator=(command_buffer&&) = default;

        /**
         * @brief Record a draw command into the command buffer.
         *
         * @param program   program to use for drawing
         * @param vertices  vertex buffer to use
         * @param indices   index buffer to use
         */
        void draw(const program_handle&       program,
                  const vertex_buffer_handle& vertices,
                  const index_buffer_handle&  indices);

        template <typename F> void for_each(F&& f) const
        {
            auto count = m_programs.size();
            for (size_t i = 0; i < count; ++i) {
                f(m_programs[i], m_vertex_buffers[i], m_index_buffers[i]);
            }
        }

        bool empty() const noexcept
        {
            return m_programs.empty();
        }

    private:
        std::vector<program_handle>       m_programs;
        std::vector<vertex_buffer_handle> m_vertex_buffers;
        std::vector<index_buffer_handle>  m_index_buffers;
    };
} // namespace mge