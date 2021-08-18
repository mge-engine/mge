// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "index_buffer.hpp"
#include "common.hpp"
#include "dx11.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {

    index_buffer::index_buffer(render_context& context,
                               mge::data_type  type,
                               size_t          data_size,
                               void*           data)
        : mge::index_buffer(context, type, data_size, data)
        , m_format(dx11_format(type))
        , m_mapped_memory(nullptr)
    {
        create_buffer(data);
    }

    void index_buffer::create_buffer(void* data)
    {
        // MGE_DEBUG_TRACE(DX11) << "Create DirectX11 buffer";
        D3D11_BUFFER_DESC buffer_desc = {};
        // TODO: #113 DirectX11 buffer usage support
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
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
        // MGE_DEBUG_TRACE(DX11) << "DirectX11 buffer created";
        mge::com_unique_ptr<ID3D11Buffer> buffer_ptr(buffer);
        m_buffer.swap(buffer_ptr);
    }

    void* index_buffer::on_map()
    {
        // only dynamic resources actually support mapping,
        // so far now we do not map here
        // TODO: #115 Support DirectX11 mapping of resources
        // MGE_DEBUG_TRACE(DX11) << "Mapping DirectX11 buffer of size " <<
        // size();
        m_mapped_memory = mge::malloc(size());
        return m_mapped_memory;
    }

    void index_buffer::on_unmap()
    {
        // MGE_DEBUG_TRACE(DX11) << "Unmapping memory";
        create_buffer(m_mapped_memory);
        // MGE_DEBUG_TRACE(DX11) << "Free unmapped memory";
        mge::free(m_mapped_memory);
        m_mapped_memory = nullptr;
    }

    index_buffer::~index_buffer()
    {
        mge::free(m_mapped_memory);
        m_mapped_memory = nullptr;
    }

} // namespace mge::dx11
