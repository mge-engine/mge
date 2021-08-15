// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "index_buffer.hpp"
#include "error.hpp"
#include "render_context.hpp"

namespace mge::opengl {
    index_buffer::index_buffer(render_context& context,
                               mge::data_type  dt,
                               size_t          data_size,
                               void*           data)
        : mge::index_buffer(context, dt, data_size, data)
        , m_buffer(0)
    {
        glCreateBuffers(1, &m_buffer);
        CHECK_OPENGL_ERROR(glCreateBuffers);
        // TODO #112 Support different index buffer usage
        glNamedBufferData(m_buffer,
                          static_cast<GLsizeiptr>(size()),
                          data,
                          GL_STATIC_DRAW);
        CHECK_OPENGL_ERROR(glNamedBufferData);
    }

    index_buffer::~index_buffer()
    {
        if (m_buffer) {
            glDeleteBuffers(1, &m_buffer);
            TRACE_OPENGL_ERROR(glDeleteBuffers);
        }
    }

    void* index_buffer::on_map()
    {
        void* result = glMapNamedBuffer(m_buffer, GL_READ_WRITE);
        CHECK_OPENGL_ERROR(glMapNamedBuffer);
        return result;
    }

    void index_buffer::on_unmap()
    {
        glUnmapNamedBuffer(m_buffer);
        CHECK_OPENGL_ERROR(glUnmapNamedBuffer);
    }

} // namespace mge::opengl