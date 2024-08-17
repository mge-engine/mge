// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "index_buffer.hpp"
#include "common.hpp"
#include "mge/core/checked_cast.hpp"

namespace mge::dx12 {
    index_buffer::index_buffer(render_context& context,
                               mge::data_type  type,
                               size_t          data_size,
                               void*           data)
        : mge::index_buffer(context, type, data_size, data)
        , m_buffer(nullptr)
        , m_mapped_memory(nullptr)
    {
        create_buffer(data);
    }

    index_buffer::~index_buffer()
    {
        mge::free(m_mapped_memory);
        m_mapped_memory = nullptr;
    }

    void* index_buffer::on_map()
    {
        m_mapped_memory = mge::malloc(size());
        return m_mapped_memory;
    }

    void index_buffer::on_unmap()
    {
        create_buffer(m_mapped_memory);
        mge::free(m_mapped_memory);
        m_mapped_memory = nullptr;
    }

    void index_buffer::create_buffer(void* data)
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

        // buffer needs to be created only once as parameters are immutable
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
        if (data) {
            ID3D12Resource* upload_buffer = nullptr;
            heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
            auto hr = dx12_context(context()).device()->CreateCommittedResource(
                &heap_properties,
                D3D12_HEAP_FLAG_NONE,
                &buffer_desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&upload_buffer));
            CHECK_HRESULT(hr, ID3D12Device, CreateCommittedResource);
            mge::com_unique_ptr<ID3D12Resource> upload_buffer_ptr(
                upload_buffer);

            void* mapped_data = nullptr;
            hr = upload_buffer->Map(0, nullptr, &mapped_data);
            CHECK_HRESULT(hr, ID3D12Resource, Map);
            memcpy(mapped_data, data, size());
            upload_buffer->Unmap(0, nullptr);

            dx12_context(context()).copy_resource_sync(
                m_buffer.get(),
                upload_buffer_ptr.get(),
                D3D12_RESOURCE_STATE_INDEX_BUFFER);

            m_buffer_view.BufferLocation = m_buffer->GetGPUVirtualAddress();
            m_buffer_view.SizeInBytes = mge::checked_cast<UINT>(size());
            m_buffer_view.Format = dx12_format(element_type());
        } else {
            m_buffer_view.BufferLocation = 0;
            m_buffer_view.SizeInBytes = 0;
            m_buffer_view.Format = dx12_format(element_type());
        }
    }
} // namespace mge::dx12