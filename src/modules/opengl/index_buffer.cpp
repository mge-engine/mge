// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "index_buffer.hpp"
#include "error.hpp"
#include "mge/core/checked_cast.hpp"
#include "render_context.hpp"
namespace mge::opengl {
    index_buffer::index_buffer(render_context& context,
                               mge::data_type  dt,
                               size_t          data_size)
        : mge::index_buffer(context, dt, data_size)
        , m_buffer(0)
    {
        glCreateBuffers(1, &m_buffer);
        CHECK_OPENGL_ERROR(glCreateBuffers);
        // TODO #112 Support different index buffer usage
        /*
        glNamedBufferData(m_buffer,
                          mge::checked_cast<GLsizeiptr>(size()),
                          data,
                          GL_STATIC_DRAW);
        CHECK_OPENGL_ERROR(glNamedBufferData);
        */
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

    void index_buffer::on_set_data(void* data, size_t size)
    {
        /*
        glNamedBufferSubData(m_buffer,
                             0,
                             mge::checked_cast<GLsizeiptr>(size),
                             data);
        CHECK_OPENGL_ERROR(glNamedBufferSubData);
        */
        MGE_THROW_NOT_IMPLEMENTED
            << "Setting data on OpenGL index buffer not implemented yet";
    }

} // namespace mge::opengl