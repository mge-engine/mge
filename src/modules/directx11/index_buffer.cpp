// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
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
                               size_t          data_size)
        : mge::index_buffer(context, type, data_size)
        , m_format(dx11_format(type))
    {
        create_buffer(nullptr);
    }

    index_buffer::~index_buffer() {}

    void index_buffer::create_buffer(void* data)
    {
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
        mge::com_unique_ptr<ID3D11Buffer> buffer_ptr(buffer);
        m_buffer.swap(buffer_ptr);
    }

    void index_buffer::on_set_data(void* data, size_t data_size)
    {
        MGE_THROW_NOT_IMPLEMENTED
            << "Setting data on DirectX11 index buffer not implemented yet";
        // check size vs data size
        // create_buffer(data);
    }

    void* index_buffer::on_map()
    {
        return nullptr;
    }

    void index_buffer::on_unmap() {}

} // namespace mge::dx11
