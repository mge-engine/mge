// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "vertex_buffer.hpp"
#include "error.hpp"
#include "mge/core/buffer.hpp"
#include "mge/core/checked_cast.hpp"
#include "render_context.hpp"

namespace mge::opengl {
    vertex_buffer::vertex_buffer(render_context&           context,
                                 const mge::vertex_layout& layout,
                                 size_t                    data_size)
        : mge::vertex_buffer(context, layout, data_size)
        , m_buffer(0)
    {
        context.prepare_frame([this]() {
            glCreateBuffers(1, &m_buffer);
            CHECK_OPENGL_ERROR(glCreateBuffers);
            set_ready(true);
        });
    }

    vertex_buffer::~vertex_buffer()
    {
        if (m_buffer) {
            set_ready(false);
            GLuint buffer_to_delete = m_buffer;
            m_context.prepare_frame([buffer_to_delete]() {
                glDeleteBuffers(1, &buffer_to_delete);
                TRACE_OPENGL_ERROR(glDeleteBuffers);
            });
            m_buffer = 0;
        }
    }

    void vertex_buffer::on_set_data(void* data, size_t data_size)
    {
        glNamedBufferSubData(m_buffer,
                             0,
                             mge::checked_cast<GLsizeiptr>(data_size),
                             data);
        CHECK_OPENGL_ERROR(glNamedBufferSubData);
        set_ready(true);
    }

} // namespace mge::opengl