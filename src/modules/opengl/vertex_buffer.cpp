// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "vertex_buffer.hpp"
#include "common.hpp"
#include "error.hpp"
namespace opengl {
    vertex_buffer::vertex_buffer(mge::render_context &     context,
                                 const mge::vertex_layout &layout,
                                 mge::usage usage, size_t element_count,
                                 void *initial_data)
        : mge::vertex_buffer(context, layout, usage, element_count,
                             initial_data),
          m_buffer(0), m_size(element_count * layout.binary_size())
    {
        await([&] {
            glCreateBuffers(1, &m_buffer);
            CHECK_OPENGL_ERROR(glCreateBuffers);
            glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
            CHECK_OPENGL_ERROR(glBindBuffer);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size()),
                         initial_data, gl_usage(usage));
            CHECK_OPENGL_ERROR(glBufferData);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            CHECK_OPENGL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
        });
    }

    vertex_buffer::~vertex_buffer()
    {
        if (m_buffer) {
            await([&] {
                glDeleteBuffers(1, &m_buffer);
                error::clear();
            });
        }
    }

    void *vertex_buffer::on_map()
    {
        void *mapped_buffer = nullptr;
        if (m_buffer) {
            await([&] {
                glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
                CHECK_OPENGL_ERROR(glBindBuffer);

                mapped_buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
                CHECK_OPENGL_ERROR(glMapBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                CHECK_OPENGL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
            });
        }
        return mapped_buffer;
    }

    void vertex_buffer::on_unmap()
    {
        if (m_buffer) {
            await([&] {
                auto rc = glUnmapNamedBuffer(m_buffer);
                if (rc == GL_FALSE) {
                    CHECK_OPENGL_ERROR(glUnmapNamedBuffer);
                }
            });
        }
    }

    size_t vertex_buffer::size() const { return m_size; }
} // namespace opengl
