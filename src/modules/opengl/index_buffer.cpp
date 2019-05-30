#include "index_buffer.hpp"

namespace opengl {
    index_buffer::index_buffer(mge::render_context &context,
                               mge::data_type type,
                               mge::buffer_usage usage,
                               mge::buffer_access cpu_access,
                               mge::buffer_access gpu_access)
        : mge::index_buffer(context,
                            type,
                            usage,
                            cpu_access,
                            gpu_access)
    {}


    index_buffer::index_buffer(mge::render_context &context,
                               mge::data_type type,
                               mge::buffer_usage usage,
                               mge::buffer_access cpu_access,
                               mge::buffer_access gpu_access,
                               void *data,
                               size_t data_size)
        : mge::index_buffer(context,
                            type,
                            usage,
                            cpu_access,
                            gpu_access,
                            data,
                            data_size)
    {}

    index_buffer::~index_buffer()
    {}

    void *
    index_buffer::on_map()
    {
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
