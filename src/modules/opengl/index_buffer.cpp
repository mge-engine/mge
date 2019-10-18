// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "index_buffer.hpp"
#include "error.hpp"
#include "common.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(OPENGL);


namespace opengl {
    index_buffer::index_buffer(mge::render_context &context,
                               mge::data_type type,
                               mge::usage usage,
                               size_t element_count,
                               void *initial_data)
        : mge::index_buffer(context,
                            type,
                            usage,
                            element_count,
                            initial_data)
        ,m_buffer(0)
        ,m_size(element_count * mge::data_type_size(type))
    {
        await([&]{
            glCreateBuffers(1, &m_buffer);
            CHECK_OPENGL_ERROR(glCreateBuffers);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
            CHECK_OPENGL_ERROR(glBindBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         static_cast<GLsizeiptr>(size()),
                         initial_data,
                         gl_usage(usage));
            CHECK_OPENGL_ERROR(glBufferData);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            CHECK_OPENGL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        });
    }


    index_buffer::~index_buffer()
    {
        if(m_buffer) {
            await([&]{
                glDeleteBuffers(1, &m_buffer);
                error::clear();
            });
        }
    }

    void *
    index_buffer::on_map()
    {
        void *mapped_buffer = nullptr;
        if(m_buffer) {
            await([&]{
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
                CHECK_OPENGL_ERROR(glBindBuffer);

                mapped_buffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);
                CHECK_OPENGL_ERROR(glMapBuffer);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
                CHECK_OPENGL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            });

        }
        return mapped_buffer;
    }

    void
    index_buffer::on_unmap()
    {
        if(m_buffer) {
            await([&]{
                auto rc = glUnmapNamedBuffer(m_buffer);
                if(rc == GL_FALSE) {
                    CHECK_OPENGL_ERROR(glUnmapNamedBuffer);
                }
            });
        }
        return;
    }

    size_t
    index_buffer::size() const
    {
        return m_size;
    }

}
