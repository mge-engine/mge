// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "mge/graphics/texture.hpp"
#include "mge/win32/com_ptr.hpp"
#include "mge/win32/com_unique_ptr.hpp"

namespace mge::dx12 {
    class render_context;

    class texture : public mge::texture
    {
    public:
        texture(render_context& context, mge::texture_type type);
        ~texture() override;

        void set_data(const mge::image_format& format,
                      const mge::extent&       extent,
                      const void*              data,
                      size_t                   size) override;

        D3D12_GPU_DESCRIPTOR_HANDLE srv_gpu_handle() const
        {
            return m_srv_gpu_handle;
        }

    private:
        DXGI_FORMAT texture_format(const mge::image_format& format) const;

        mge::com_ptr<ID3D12Resource> m_texture;
        D3D12_GPU_DESCRIPTOR_HANDLE  m_srv_gpu_handle;
        D3D12_CPU_DESCRIPTOR_HANDLE  m_srv_cpu_handle;
    };
} // namespace mge::dx12
