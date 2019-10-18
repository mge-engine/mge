// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "index_buffer.hpp"
#include "render_context.hpp"
#include "error.hpp"
#include "common.hpp"
#include "mge/core/numeric_cast.hpp"
#include "mge/core/memory.hpp"

namespace dx11 {

    index_buffer::index_buffer(mge::render_context& context,
                               mge::data_type type,
                               mge::usage usage,
                               size_t element_count,
                               void  *initial_data)
        :mge::index_buffer(context,
                           type,
                           usage,
                           element_count,
                           initial_data)
        ,m_format(dx11_format(type))
        ,m_size(element_count * mge::data_type_size(type))
    {
        if(initial_data) {
            create_buffer(initial_data);
        }
    }


    void
    index_buffer::create_buffer(void *data)
    {
        D3D11_BUFFER_DESC buffer_desc = {};
        buffer_desc.Usage     = dx11_usage(buffer_usage());
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        buffer_desc.ByteWidth = mge::numeric_cast<UINT>(size());
        buffer_desc.CPUAccessFlags = buffer_usage() == mge::usage::STAGING ? D3D11_CPU_ACCESS_READ : 0;
        buffer_desc.MiscFlags = 0;
        D3D11_SUBRESOURCE_DATA init_data = {};
        init_data.pSysMem = data;
        ID3D11Buffer *buffer = nullptr;
        auto hr = dx11_device(context())->CreateBuffer(&buffer_desc,
                                                       &init_data,
                                                       &buffer);
        CHECK_HRESULT(hr, ID3D11Device, CreateBuffer);
        m_buffer.reset(buffer);
    }

    index_buffer::~index_buffer()
    {
        if(m_mapped_memory) {
            mge::free(m_mapped_memory);
        }
    }

    size_t
    index_buffer::size() const noexcept
    {
        return m_size;
    }

    void *
    index_buffer::on_map()
    {
        m_buffer.reset();
        void *mapped_memory = mge::malloc(size());
        return mapped_memory;
    }

    void
    index_buffer::on_unmap()
    {
        create_buffer(m_mapped_memory);
        mge::free(m_mapped_memory);
    }
}
