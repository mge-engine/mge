// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "vertex_buffer.hpp"

namespace mge::dx11 {
    vertex_buffer::vertex_buffer(render_context&      context,
                                 const vertex_layout& layout,
                                 size_t               data_size,
                                 void*                initial_data)
        : mge::vertex_buffer(context, layout, data_size, initial_data)
        , m_mapped_memory(nullptr)
    {}

    vertex_buffer::~vertex_buffer() {}

    void* vertex_buffer::on_map()
    {
        // only dynamic resources actually support mapping,
        // so far now we do not map here
        // TODO: #115 Support DirectX11 mapping of resources
        // MGE_DEBUG_TRACE(DX11) << "Mapping DirectX11 buffer of size " <<
        // size();
        m_mapped_memory = mge::malloc(size());
        return m_mapped_memory;
    }

    void vertex_buffer::on_unmap()
    {
        mge::free(m_mapped_memory);
        m_mapped_memory = nullptr;
    }

    void vertex_buffer::create_buffer(void* data) {}

} // namespace mge::dx11