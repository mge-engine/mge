#include "index_buffer.hpp"
#include "error.hpp"
#include "common.hpp"

namespace opengl {
    index_buffer::index_buffer(mge::render_context &context,
                               mge::data_type type,
                               mge::buffer_change_policy change_policy,
                               mge::buffer_access cpu_access,
                               mge::buffer_access gpu_access,
                               size_t element_count,
                               void *initial_data)
        : mge::index_buffer(context,
                            type,
                            change_policy,
                            cpu_access,
                            gpu_access,
                            element_count,
                            initial_data)
        ,m_buffer(0)
    {
        await([&]{
            glCreateBuffers(1, &m_buffer);
            CHECK_OPENGL_ERROR(glCreateBuffers);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
            CHECK_OPENGL_ERROR(glBindBuffer);

            glBufferData(GL_ARRAY_BUFFER, size(), initial_data, gl_buffer_change_policy(change_policy));
            CHECK_OPENGL_ERROR(glBufferData);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
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
                mapped_buffer = glMapNamedBuffer(m_buffer, gl_buffer_access(cpu_access()));
                CHECK_OPENGL_ERROR(glMapNamedBuffer);
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
        return 0;
    }

}
