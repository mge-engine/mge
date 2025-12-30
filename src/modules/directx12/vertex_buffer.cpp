// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "vertex_buffer.hpp"
#include "error.hpp"
#include "mge/core/checked_cast.hpp"
#include "render_context.hpp"

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {
    vertex_buffer::vertex_buffer(render_context&      context,
                                 const vertex_layout& layout,
                                 size_t               data_size)
        : mge::vertex_buffer(context, layout, data_size)
    {
        context.prepare_frame([this]() { create_buffer(); });
    }

    vertex_buffer::~vertex_buffer() {}

    void vertex_buffer::on_set_data(void* data, size_t size)
    {
        if (!data || size == 0) {
            return;
        }

        if (size > this->size()) {
            MGE_THROW(mge::illegal_argument)
                << "Data size " << size << " exceeds buffer size "
                << this->size();
        }

        // create upload buffer
        mge::com_unique_ptr<ID3D12Resource> upload_buffer;
        D3D12_HEAP_PROPERTIES               heap_properties = {};
        heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;

        D3D12_RESOURCE_DESC buffer_desc = {};
        buffer_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        buffer_desc.Width = mge::checked_cast<UINT>(size);
        buffer_desc.Height = 1;
        buffer_desc.DepthOrArraySize = 1;
        buffer_desc.MipLevels = 1;
        buffer_desc.Format = DXGI_FORMAT_UNKNOWN;
        buffer_desc.SampleDesc.Count = 1;
        buffer_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        buffer_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        ID3D12Resource* upload_buf = nullptr;
        auto hr = dx12_context(context()).device()->CreateCommittedResource(
            &heap_properties,
            D3D12_HEAP_FLAG_NONE,
            &buffer_desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&upload_buf));
        CHECK_HRESULT(hr, ID3D12Device, CreateCommittedResource);
        upload_buffer.reset(upload_buf);

        // copy data to upload buffer
        void* mapped_data = nullptr;
        hr = upload_buffer->Map(0, nullptr, &mapped_data);
        CHECK_HRESULT(hr, ID3D12Resource, Map);
        memcpy(mapped_data, data, size);
        upload_buffer->Unmap(0, nullptr);

        // copy from upload buffer to vertex buffer
        dx12_context(context()).copy_resource_sync(
            m_buffer.get(),
            upload_buffer.get(),
            D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        m_buffer_view.BufferLocation = m_buffer->GetGPUVirtualAddress();
        m_buffer_view.SizeInBytes = mge::checked_cast<UINT>(size);
        m_buffer_view.StrideInBytes =
            mge::checked_cast<UINT>(layout().stride());
    }

    void vertex_buffer::create_buffer()
    {
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

        if (!m_buffer) {
            ID3D12Resource* buffer = nullptr;
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
        m_buffer_view.BufferLocation = 0;
        m_buffer_view.SizeInBytes = 0;
        m_buffer_view.StrideInBytes = 0;
    }

} // namespace mge::dx12