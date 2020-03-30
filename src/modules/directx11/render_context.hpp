// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/graphics/render_context.hpp"
#include "system_config.hpp"
#include "win32/com_unique_ptr.hpp"

namespace dx11 {

    class window;

    class render_context : public mge::render_context
    {
    public:
        render_context(window *win, const system_config &config);
        ~render_context();
        void flush() override;
        mge::vertex_buffer_ref
                              create_vertex_buffer(const mge::vertex_layout &layout, mge::usage usage,
                                                   size_t element_count, void *initial_data) override;
        mge::index_buffer_ref create_index_buffer(mge::data_type type,
                                                  mge::usage     usage,
                                                  size_t         element_count,
                                                  void *initial_data) override;
        mge::texture_2d_ref
                            create_texture_2d(const mge::image_ref &image) override;
        mge::texture_2d_ref create_texture_2d() override;
        mge::shader_program_ref
                              create_shader_program(mge::shader_type type) override;
        mge::pipeline_ref     create_pipeline() override;
        mge::command_list_ref create_command_list() override;
        void execute(const mge::command_list_ref &commands) override;
        void shader_languages(
            std::vector<mge::shader_language> &languages) const override;
        ID3D11Device *       device() const noexcept { return m_device.get(); }
        ID3D11DeviceContext *device_context() const noexcept
        {
            return m_device_context.get();
        }
        ID3D11RenderTargetView *render_target_view() const noexcept
        {
            return m_render_target_view.get();
        }

    private:
        void init_context(const system_config &config);

        window *m_window;

        mge::com_unique_ptr<ID3D11Device>            m_device;
        mge::com_unique_ptr<ID3D11DeviceContext>     m_device_context;
        mge::com_unique_ptr<ID3D11Texture2D>         m_depth_stencil_buffer;
        mge::com_unique_ptr<ID3D11DepthStencilState> m_depth_stencil_state;
        mge::com_unique_ptr<ID3D11RenderTargetView>  m_render_target_view;
        mge::com_unique_ptr<IDXGISwapChain>          m_swap_chain;
    };

    inline ID3D11Device *dx11_device(const mge::render_context &context)
    {
        return static_cast<const render_context &>(context).device();
    }

    inline ID3D11DeviceContext *
    dx11_device_context(const mge::render_context &context)
    {
        return static_cast<const render_context &>(context).device_context();
    }

    inline ID3D11RenderTargetView *
    dx11_render_target_view(const mge::render_context &context)
    {
        return static_cast<const render_context &>(context)
            .render_target_view();
    }
} // namespace dx11
