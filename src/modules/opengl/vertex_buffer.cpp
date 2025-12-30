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
        });
        glCreateBuffers(1, &m_buffer);
        CHECK_OPENGL_ERROR(glCreateBuffers);
    }

    vertex_buffer::~vertex_buffer()
    {
        if (m_buffer) {
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
        mge::buffer_ref data_buffer = std::make_shared<mge::buffer>(
            static_cast<uint8_t*>(data),
            static_cast<uint8_t*>(data) + data_size);
        context().prepare_frame([this, data_buffer, data_size]() {
            glNamedBufferSubData(m_buffer,
                                 0,
                                 mge::checked_cast<GLsizeiptr>(data_size),
                                 data_buffer->data());
            CHECK_OPENGL_ERROR(glNamedBufferSubData);
        });
    }

} // namespace mge::opengl