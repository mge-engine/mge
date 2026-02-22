// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "texture.hpp"
#include "error.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {

    texture::texture(render_context& context, mge::texture_type type)
        : mge::texture(context, type)
    {
        m_srv_gpu_handle = {};
        m_srv_cpu_handle = {};
    }

    texture::~texture() {}

    DXGI_FORMAT texture::texture_format(const mge::image_format& format) const
    {
        switch (format.format()) {
        case mge::image_format::data_format::RGB:
            switch (format.type()) {
            case mge::data_type::FLOAT:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            default:
                MGE_THROW(mge::illegal_argument)
                    << "Unsupported image format (data type): " << format;
            }
        case mge::image_format::data_format::RGBA:
            switch (format.type()) {
            case mge::data_type::UINT8:
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            case mge::data_type::UINT16:
                return DXGI_FORMAT_R16G16B16A16_UNORM;
            case mge::data_type::FLOAT:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            default:
                MGE_THROW(mge::illegal_argument)
                    << "Unsupported image format (data type): " << format;
            }
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unsupported image format (format): " << format;
        }
    }

    void texture::set_data(const mge::image_format& format,
                           const mge::extent&       extent,
                           const void*              data,
                           size_t                   size)
    {
        auto& ctx = static_cast<render_context&>(context());
        auto* device = ctx.device();

        DXGI_FORMAT dxgi_format = texture_format(format);
        UINT        row_pitch =
            mge::checked_cast<UINT>(extent.width * format.binary_size());

        // Create the texture resource on the default heap
        D3D12_RESOURCE_DESC tex_desc = {};
        tex_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        tex_desc.Alignment = 0;
        tex_desc.Width = extent.width;
        tex_desc.Height = extent.height;
        tex_desc.DepthOrArraySize = 1;
        tex_desc.MipLevels = 1;
        tex_desc.Format = dxgi_format;
        tex_desc.SampleDesc.Count = 1;
        tex_desc.SampleDesc.Quality = 0;
        tex_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        tex_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        D3D12_HEAP_PROPERTIES default_heap = {};
        default_heap.Type = D3D12_HEAP_TYPE_DEFAULT;

        auto hr =
            device->CreateCommittedResource(&default_heap,
                                            D3D12_HEAP_FLAG_NONE,
                                            &tex_desc,
                                            D3D12_RESOURCE_STATE_COPY_DEST,
                                            nullptr,
                                            IID_PPV_ARGS(&m_texture));
        CHECK_HRESULT(hr, ID3D12Device, CreateCommittedResource);

        // Determine the upload buffer layout
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
        UINT                               num_rows = 0;
        UINT64                             row_size_in_bytes = 0;
        UINT64                             total_bytes = 0;
        device->GetCopyableFootprints(&tex_desc,
                                      0,
                                      1,
                                      0,
                                      &footprint,
                                      &num_rows,
                                      &row_size_in_bytes,
                                      &total_bytes);

        // Create upload buffer
        D3D12_HEAP_PROPERTIES upload_heap = {};
        upload_heap.Type = D3D12_HEAP_TYPE_UPLOAD;

        D3D12_RESOURCE_DESC upload_desc = {};
        upload_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        upload_desc.Width = total_bytes;
        upload_desc.Height = 1;
        upload_desc.DepthOrArraySize = 1;
        upload_desc.MipLevels = 1;
        upload_desc.Format = DXGI_FORMAT_UNKNOWN;
        upload_desc.SampleDesc.Count = 1;
        upload_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        upload_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        mge::com_unique_ptr<ID3D12Resource> upload_buffer;
        ID3D12Resource*                     upload_buf = nullptr;
        hr = device->CreateCommittedResource(&upload_heap,
                                             D3D12_HEAP_FLAG_NONE,
                                             &upload_desc,
                                             D3D12_RESOURCE_STATE_GENERIC_READ,
                                             nullptr,
                                             IID_PPV_ARGS(&upload_buf));
        CHECK_HRESULT(hr, ID3D12Device, CreateCommittedResource);
        upload_buffer.reset(upload_buf);

        // Copy data to upload buffer, respecting row pitch alignment
        void* mapped_data = nullptr;
        hr = upload_buffer->Map(0, nullptr, &mapped_data);
        CHECK_HRESULT(hr, ID3D12Resource, Map);

        const uint8_t* src_ptr = static_cast<const uint8_t*>(data);
        uint8_t*       dst_ptr =
            static_cast<uint8_t*>(mapped_data) + footprint.Offset;
        for (UINT row = 0; row < num_rows; ++row) {
            memcpy(dst_ptr + row * footprint.Footprint.RowPitch,
                   src_ptr + row * row_pitch,
                   row_pitch);
        }
        upload_buffer->Unmap(0, nullptr);

        // Copy texture from upload buffer to default heap using the transfer
        // command list
        ctx.copy_texture_sync(m_texture.Get(), upload_buffer.get(), footprint);

        // Create SRV
        auto srv_handles = ctx.allocate_srv();
        m_srv_cpu_handle = srv_handles.first;
        m_srv_gpu_handle = srv_handles.second;

        D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
        srv_desc.Shader4ComponentMapping =
            D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srv_desc.Format = dxgi_format;
        srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D.MostDetailedMip = 0;
        srv_desc.Texture2D.MipLevels = 1;

        device->CreateShaderResourceView(m_texture.Get(),
                                         &srv_desc,
                                         m_srv_cpu_handle);
    }
} // namespace mge::dx12
