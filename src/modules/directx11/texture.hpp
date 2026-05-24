// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/graphics/texture.hpp"
#include "mge/win32/com_unique_ptr.hpp"

namespace mge::dx11 {
    class render_context;

    class texture : public mge::texture
    {
    public:
        texture(render_context& context, mge::texture_type type);
        texture(render_context&          context,
                mge::texture_type        type,
                const mge::image_format& format,
                const mge::extent&       extent);
        ~texture() override;

        void set_data(const mge::image_format& format,
                      const mge::extent&       extent,
                      const void*              data,
                      size_t                   size) override;

        ID3D11ShaderResourceView* shader_resource_view() const
        {
            return m_shader_resource_view.get();
        }

        ID3D11SamplerState* sampler_state() const
        {
            return m_sampler_state.get();
        }

        ID3D11RenderTargetView* render_target_view() const
        {
            return m_render_target_view.get();
        }

    private:
        DXGI_FORMAT texture_format(const mge::image_format& format) const;
        mge::com_unique_ptr<ID3D11Texture2D>          m_texture;
        mge::com_unique_ptr<ID3D11ShaderResourceView> m_shader_resource_view;
        mge::com_unique_ptr<ID3D11SamplerState>       m_sampler_state;
        mge::com_unique_ptr<ID3D11RenderTargetView>   m_render_target_view;
    };
} // namespace mge::dx11
