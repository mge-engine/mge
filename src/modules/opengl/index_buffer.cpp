#include "index_buffer.hpp"
#include "error.hpp"

namespace opengl {
    index_buffer::index_buffer(mge::render_context &context,
                               mge::data_type type,
                               mge::buffer_usage usage,
                               mge::buffer_access cpu_access,
                               mge::buffer_access gpu_access,
                               size_t element_count,
                               void *initial_data)
        : mge::index_buffer(context,
                            type,
                            usage,
                            cpu_access,
                            gpu_access,
                            element_count,
                            initial_data)
        ,m_buffer(0)
    {
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
        if(m_buffer) {

        }
        return nullptr;
    }

    void
    index_buffer::on_unmap()
    {
        return;
    }

    size_t
    index_buffer::size() const
    {
        return 0;
    }

}
