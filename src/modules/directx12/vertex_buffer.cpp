// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "vertex_buffer.hpp"
#include "error.hpp"
#include "mge/core/checked_cast.hpp"
#include "render_context.hpp"

namespace mge::dx12 {
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

    void vertex_buffer::create_buffer(void* data)
    {
        ID3D12Resource*       buffer = nullptr;
        D3D12_HEAP_PROPERTIES heap_properties = {};
        heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
        D3D12_RESOURCE_DESC buffer_desc = {};
        buffer_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        buffer_desc.Width = mge::checked_cast<UINT>(size());
        buffer_desc.Height = 1;
        buffer_desc.DepthOrArraySize = 1;
        buffer_desc.MipLevels = 1;
        buffer_desc.Format = DXGI_FORMAT_UNKNOWN;
        buffer_desc.SampleDesc.Count = 1;
        buffer_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        buffer_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        auto hr = dx12_context(context()).device()->CreateCommittedResource(
            &heap_properties,
            D3D12_HEAP_FLAG_NONE,
            &buffer_desc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&buffer));
        CHECK_HRESULT(hr, ID3D12Device, CreateCommittedResource);
        m_buffer.reset(buffer);
    }

} // namespace mge::dx12