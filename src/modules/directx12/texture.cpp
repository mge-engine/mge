// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
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
        m_rtv_cpu_handle = {};
        m_dsv_cpu_handle = {};
    }

    texture::texture(render_context&          context,
                     mge::texture_type        type,
                     const mge::image_format& format,
                     const mge::extent&       extent)
        : mge::texture(context, type, mge::texture_usage::RENDER_TARGET)
    {
        m_srv_gpu_handle = {};
        m_srv_cpu_handle = {};
        m_rtv_cpu_handle = {};
        m_dsv_cpu_handle = {};

        auto& ctx = static_cast<render_context&>(context);
        auto* device = ctx.device();

        bool is_depth =
            format.format() == mge::image_format::data_format::DEPTH ||
            format.format() == mge::image_format::data_format::DEPTH_STENCIL;

        D3D12_HEAP_PROPERTIES default_heap = {};
        default_heap.Type = D3D12_HEAP_TYPE_DEFAULT;

        if (is_depth) {
            DXGI_FORMAT typeless_fmt;
            DXGI_FORMAT dsv_fmt;
            DXGI_FORMAT srv_fmt;

            if (format.format() ==
                mge::image_format::data_format::DEPTH_STENCIL) {
                typeless_fmt = DXGI_FORMAT_R24G8_TYPELESS;
                dsv_fmt = DXGI_FORMAT_D24_UNORM_S8_UINT;
                srv_fmt = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            } else if (format.type() == mge::data_type::FLOAT) {
                typeless_fmt = DXGI_FORMAT_R32_TYPELESS;
                dsv_fmt = DXGI_FORMAT_D32_FLOAT;
                srv_fmt = DXGI_FORMAT_R32_FLOAT;
            } else if (format.type() == mge::data_type::UINT16) {
                typeless_fmt = DXGI_FORMAT_R16_TYPELESS;
                dsv_fmt = DXGI_FORMAT_D16_UNORM;
                srv_fmt = DXGI_FORMAT_R16_UNORM;
            } else {
                MGE_THROW(mge::illegal_argument)
                    << "Unsupported depth image format (data type): " << format;
            }
            m_view_dxgi_format = dsv_fmt;

            D3D12_RESOURCE_DESC tex_desc = {};
            tex_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            tex_desc.Width = extent.width;
            tex_desc.Height = extent.height;
            tex_desc.DepthOrArraySize = 1;
            tex_desc.MipLevels = 1;
            tex_desc.Format = typeless_fmt;
            tex_desc.SampleDesc.Count = 1;
            tex_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            tex_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

            D3D12_CLEAR_VALUE clear_value = {};
            clear_value.Format = dsv_fmt;
            clear_value.DepthStencil.Depth = 1.0f;
            clear_value.DepthStencil.Stencil = 0;

            auto hr = device->CreateCommittedResource(
                &default_heap,
                D3D12_HEAP_FLAG_NONE,
                &tex_desc,
                D3D12_RESOURCE_STATE_DEPTH_WRITE,
                &clear_value,
                IID_PPV_ARGS(m_texture.GetAddressOf()));
            CHECK_HRESULT(hr, ID3D12Device, CreateCommittedResource);

            m_dsv_cpu_handle = ctx.allocate_dsv();
            D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
            dsv_desc.Format = dsv_fmt;
            dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            dsv_desc.Flags = D3D12_DSV_FLAG_NONE;
            device->CreateDepthStencilView(m_texture.Get(), &dsv_desc,
                                           m_dsv_cpu_handle);

            auto srv_handles = ctx.allocate_srv();
            m_srv_cpu_handle = srv_handles.first;
            m_srv_gpu_handle = srv_handles.second;

            D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
            srv_desc.Shader4ComponentMapping =
                D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srv_desc.Format = srv_fmt;
            srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Texture2D.MostDetailedMip = 0;
            srv_desc.Texture2D.MipLevels = 1;
            device->CreateShaderResourceView(m_texture.Get(), &srv_desc,
                                             m_srv_cpu_handle);
        } else {
            DXGI_FORMAT dxgi_format = texture_format(format);
            m_view_dxgi_format = dxgi_format;

            D3D12_RESOURCE_DESC tex_desc = {};
            tex_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            tex_desc.Width = extent.width;
            tex_desc.Height = extent.height;
            tex_desc.DepthOrArraySize = 1;
            tex_desc.MipLevels = 1;
            tex_desc.Format = dxgi_format;
            tex_desc.SampleDesc.Count = 1;
            tex_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            tex_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

            D3D12_CLEAR_VALUE clear_value = {};
            clear_value.Format = dxgi_format;
            clear_value.Color[0] = 0.0f;
            clear_value.Color[1] = 0.0f;
            clear_value.Color[2] = 0.0f;
            clear_value.Color[3] = 1.0f;

            auto hr = device->CreateCommittedResource(
                &default_heap,
                D3D12_HEAP_FLAG_NONE,
                &tex_desc,
                D3D12_RESOURCE_STATE_RENDER_TARGET,
                &clear_value,
                IID_PPV_ARGS(m_texture.GetAddressOf()));
            CHECK_HRESULT(hr, ID3D12Device, CreateCommittedResource);

            m_rtv_cpu_handle = ctx.allocate_rtv();
            D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
            rtv_desc.Format = dxgi_format;
            rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            rtv_desc.Texture2D.MipSlice = 0;
            rtv_desc.Texture2D.PlaneSlice = 0;
            device->CreateRenderTargetView(m_texture.Get(), &rtv_desc,
                                           m_rtv_cpu_handle);

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
            device->CreateShaderResourceView(m_texture.Get(), &srv_desc,
                                             m_srv_cpu_handle);
        }
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
    // Note: depth formats are handled directly in the render-target constructor
    // using typeless resource formats; texture_format() is only for colour paths.

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
