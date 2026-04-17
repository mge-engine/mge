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
        context.prepare_frame([this]() {
            glGenBuffers(1, &this->m_buffer);
            CHECK_OPENGL_ERROR(glGenBuffers);
            set_ready(true);
        });
    }

    index_buffer::~index_buffer()
    {
        if (m_buffer) {
            set_ready(false);
            GLuint buffer_to_del = m_buffer;
            context().prepare_frame([buffer_to_del]() {
                glDeleteBuffers(1, &buffer_to_del);
                TRACE_OPENGL_ERROR(glDeleteBuffers);
            });
        }
    }
    void index_buffer::on_set_data(void* data, size_t size)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
        CHECK_OPENGL_ERROR(glBindBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     mge::checked_cast<GLsizeiptr>(size),
                     data,
                     GL_DYNAMIC_DRAW);
        CHECK_OPENGL_ERROR(glBufferData);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        set_ready(true);
    }

} // namespace mge::opengl