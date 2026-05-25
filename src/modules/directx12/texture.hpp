// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "mge/graphics/texture.hpp"
#include "mge/win32/com_ptr.hpp"
#include "mge/win32/com_unique_ptr.hpp"

namespace mge::dx12 {
    class render_context_base;

    class texture : public mge::texture
    {
    public:
        texture(render_context_base& context, mge::texture_type type);
        texture(render_context_base&     context,
                mge::texture_type        type,
                const mge::image_format& format,
                const mge::extent&       extent);
        ~texture() override;

        void set_data(const mge::image_format& format,
                      const mge::extent&       extent,
                      const void*              data,
                      size_t                   size) override;

        ID3D12Resource* resource() const
        {
            return m_texture.Get();
        }

        D3D12_GPU_DESCRIPTOR_HANDLE srv_gpu_handle() const
        {
            return m_srv_gpu_handle;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE rtv_cpu_handle() const
        {
            return m_rtv_cpu_handle;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE dsv_cpu_handle() const
        {
            return m_dsv_cpu_handle;
        }

        DXGI_FORMAT view_dxgi_format() const
        {
            return m_view_dxgi_format;
        }

    private:
        DXGI_FORMAT texture_format(const mge::image_format& format) const;

        mge::com_ptr<ID3D12Resource> m_texture;
        D3D12_GPU_DESCRIPTOR_HANDLE  m_srv_gpu_handle;
        D3D12_CPU_DESCRIPTOR_HANDLE  m_srv_cpu_handle;
        D3D12_CPU_DESCRIPTOR_HANDLE  m_rtv_cpu_handle;
        D3D12_CPU_DESCRIPTOR_HANDLE  m_dsv_cpu_handle;
        DXGI_FORMAT                  m_view_dxgi_format{DXGI_FORMAT_UNKNOWN};
    };
} // namespace mge::dx12
