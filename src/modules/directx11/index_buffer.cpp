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
        context.prepare_frame([this]() { this->create_buffer(); });
    }

    index_buffer::~index_buffer() {}

    void index_buffer::create_buffer()
    {
        D3D11_BUFFER_DESC buffer_desc = {};
        // TODO: #113 DirectX11 buffer usage support
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        buffer_desc.ByteWidth = mge::checked_cast<UINT>(size());
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;

        ID3D11Buffer* buffer = nullptr;
        auto hr = dx11_context(context()).device()->CreateBuffer(&buffer_desc,
                                                                 nullptr,
                                                                 &buffer);
        CHECK_HRESULT(hr, ID3D11Device, CreateBuffer);
        mge::com_unique_ptr<ID3D11Buffer> buffer_ptr(buffer);
        m_buffer.swap(buffer_ptr);
    }

    void index_buffer::on_set_data(void* data, size_t data_size)
    {
        if (!data || data_size == 0) {
            return;
        }

        if (data_size > size()) {
            MGE_THROW(mge::illegal_argument)
                << "Data size " << data_size << " exceeds buffer size "
                << size();
        }

        D3D11_BOX dest_box;
        dest_box.left = 0;
        dest_box.right = mge::checked_cast<UINT>(data_size);
        dest_box.top = 0;
        dest_box.bottom = 1;
        dest_box.front = 0;
        dest_box.back = 1;

        dx11_context(context())
            .device_context()
            ->UpdateSubresource(m_buffer.get(), 0, &dest_box, data, 0, 0);
    }

} // namespace mge::dx11
