// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include "vertex_buffer.hpp"
#include "common.hpp"
#include "mge/core/checked_cast.hpp"

namespace mge::dx11 {
    vertex_buffer::vertex_buffer(render_context&      context,
                                 const vertex_layout& layout,
                                 size_t               data_size,
                                 void*                initial_data)
        : mge::vertex_buffer(context, layout, data_size, initial_data)
        , m_mapped_memory(nullptr)
    {
        create_buffer(initial_data);
    }

    vertex_buffer::~vertex_buffer()
    {
        mge::free(m_mapped_memory);
        m_mapped_memory = nullptr;
    }

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
        create_buffer(m_mapped_memory);
        mge::free(m_mapped_memory);
        m_mapped_memory = nullptr;
    }

    void vertex_buffer::create_buffer(void* data)
    {
        D3D11_BUFFER_DESC buffer_desc = {};
        // TODO: #113 DirectX11 buffer usage support
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buffer_desc.ByteWidth = mge::checked_cast<UINT>(size());
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initial_data = {};
        initial_data.pSysMem = data;

        ID3D11Buffer* buffer = nullptr;
        auto          hr = dx11_context(context()).device()->CreateBuffer(
            &buffer_desc,
            data ? &initial_data : nullptr,
            &buffer);
        CHECK_HRESULT(hr, ID3D11Device, CreateBuffer);
        mge::com_unique_ptr<ID3D11Buffer> buffer_ptr(buffer);
        m_buffer.swap(buffer_ptr);
    }

} // namespace mge::dx11