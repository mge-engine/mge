// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "vertex_buffer.hpp"
#include "error.hpp"
#include "mge/core/checked_cast.hpp"
#include "render_context.hpp"

namespace mge::opengl {
    vertex_buffer::vertex_buffer(render_context&           context,
                                 const mge::vertex_layout& layout,
                                 size_t                    data_size)
        : mge::vertex_buffer(context, layout, data_size)
        , m_buffer(0)
    {
        glCreateBuffers(1, &m_buffer);
        CHECK_OPENGL_ERROR(glCreateBuffers);
        MGE_THROW_NOT_IMPLEMENTED
            << "vertex_buffer constructor not fully implemented";
        /*
        // TODO #112 Support different index buffer usage
        glNamedBufferData(m_buffer,
                          mge::checked_cast<GLsizeiptr>(size()),
                          data,
                          GL_STATIC_DRAW);
        CHECK_OPENGL_ERROR(glNamedBufferData);
        */
    }

    vertex_buffer::~vertex_buffer()
    {
        if (m_buffer) {
            glDeleteBuffers(1, &m_buffer);
            TRACE_OPENGL_ERROR(glDeleteBuffers);
        }
    }

    void* vertex_buffer::on_map()
    {
        void* result = glMapNamedBuffer(m_buffer, GL_READ_WRITE);
        CHECK_OPENGL_ERROR(glMapNamedBuffer);
        return result;
    }

    void vertex_buffer::on_unmap()
    {
        glUnmapNamedBuffer(m_buffer);
        CHECK_OPENGL_ERROR(glUnmapNamedBuffer);
    }

    void vertex_buffer::on_set_data(void* data, size_t data_size)
    {
        MGE_THROW_NOT_IMPLEMENTED
            << "OpenGL vertex_buffer::on_set_data not implemented";
        /*
        glNamedBufferSubData(m_buffer,
                             0,
                             mge::checked_cast<GLsizeiptr>(data_size),
                             data);
        CHECK_OPENGL_ERROR(glNamedBufferSubData);
        */
    }

} // namespace mge::opengl