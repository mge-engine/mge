#include "index_buffer.hpp"
#include "render_context.hpp"

namespace mge::vulkan {

    index_buffer::index_buffer(render_context& context,
                               mge::data_type  type,
                               size_t          data_size,
                               void*           data)
        : mge::index_buffer(context, type, data_size, data)
        , m_vulkan_context(context)
    {}

    index_buffer::~index_buffer() {}

    void* index_buffer::on_map() { return nullptr; }

    void index_buffer::on_unmap() {}

} // namespace mge::vulkan