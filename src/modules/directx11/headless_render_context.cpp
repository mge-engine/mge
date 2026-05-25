// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "headless_render_context.hpp"
#include "error.hpp"
#include "frame_buffer.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/memory_image.hpp"
#include "render_system.hpp"
#include "texture.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {

    headless_render_context::headless_render_context(
        mge::dx11::render_system& system, const mge::extent& ext)
        : render_context_base(system, ext)
    {}

    void headless_render_context::initialize()
    {
        MGE_DEBUG_TRACE(DX11, "Initialize headless render context");

        UINT            flags = m_render_system.debug() ? D3D11_CREATE_DEVICE_DEBUG : 0;
        D3D_DRIVER_TYPE driver_type = m_render_system.software_device()
                                          ? D3D_DRIVER_TYPE_WARP
                                          : D3D_DRIVER_TYPE_HARDWARE;

        ID3D11Device*        tmp_device         = nullptr;
        ID3D11DeviceContext* tmp_device_context = nullptr;

        HRESULT rc = D3D11CreateDevice(nullptr,
                                       driver_type,
                                       nullptr,
                                       flags,
                                       nullptr,
                                       0,
                                       D3D11_SDK_VERSION,
                                       &tmp_device,
                                       nullptr,
                                       &tmp_device_context);
        CHECK_HRESULT(rc, , D3D11CreateDevice);

        m_device.reset(tmp_device);
        m_device_context.reset(tmp_device_context);

        create_default_depth_stencil_state();
        init_capabilities();
    }

    void headless_render_context::on_frame_present()
    {
        // no-op: headless context has no swap chain
    }

    mge::image_ref headless_render_context::screenshot()
    {
        for (auto* fb : m_frame_buffers) {
            if (!fb) {
                continue;
            }
            auto tex = fb->color_attachment(0);
            if (!tex) {
                continue;
            }
            auto* dx11_tex = static_cast<const dx11::texture*>(tex.get());
            auto* tex2d    = dx11_tex->texture2d();
            if (!tex2d) {
                continue;
            }

            D3D11_TEXTURE2D_DESC desc;
            tex2d->GetDesc(&desc);

            desc.Usage          = D3D11_USAGE_STAGING;
            desc.BindFlags      = 0;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            desc.MiscFlags      = 0;

            ID3D11Texture2D* staging = nullptr;
            auto             rc = m_device->CreateTexture2D(&desc, nullptr, &staging);
            CHECK_HRESULT(rc, ID3D11Device, CreateTexture2D);
            com_unique_ptr<ID3D11Texture2D> staging_ptr(staging);

            m_device_context->CopyResource(staging, tex2d);

            D3D11_MAPPED_SUBRESOURCE mapped;
            rc = m_device_context->Map(staging, 0, D3D11_MAP_READ, 0, &mapped);
            CHECK_HRESULT(rc, ID3D11DeviceContext, Map);

            mge::image_format fmt(mge::image_format::data_format::RGBA,
                                  mge::data_type::UINT8);
            auto img = std::make_shared<mge::memory_image>(
                fmt, mge::extent(desc.Width, desc.Height));

            uint32_t     row_size = desc.Width * 4;
            auto*        dst      = static_cast<uint8_t*>(img->data());
            const auto*  src      = static_cast<const uint8_t*>(mapped.pData);
            for (uint32_t y = 0; y < desc.Height; ++y) {
                memcpy(dst + y * row_size, src + y * mapped.RowPitch, row_size);
            }
            m_device_context->Unmap(staging, 0);
            return img;
        }
        return nullptr;
    }

} // namespace mge::dx11
