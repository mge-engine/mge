// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/win32/com_unique_ptr.hpp"

namespace mge::dx11 {

    class render_system;
    class window;

    class render_context : public mge::render_context
    {
    public:
        render_context(render_system& system, window& window_);
        virtual ~render_context();
        void initialize();

        mge::index_buffer* create_index_buffer(mge::data_type dt,
                                               size_t         data_size,
                                               void*          data) override;

        void destroy_index_buffer(mge::index_buffer* ib) override;

        mge::vertex_buffer_ref
        create_vertex_buffer(const mge::vertex_layout& layout,
                             size_t                    data_size,
                             void*                     data) override;

        mge::shader_ref       create_shader(mge::shader_type t) override;
        mge::program_ref      create_program() override;
        mge::command_list_ref create_command_list() override;
        mge::texture_ref      create_texture(mge::texture_type type) override;

        const ::mge::dx11::window& window() const
        {
            return m_window;
        }

        const ::mge::dx11::render_system& render_system() const
        {
            return m_render_system;
        }

        ID3D11Device* device() const
        {
            return m_device.get();
        }

        ID3D11DeviceContext* device_context() const
        {
            return m_device_context.get();
        }

        ID3D11RenderTargetView* render_target_view() const
        {
            return m_render_target_view.get();
        }

        ID3D11DepthStencilView* depth_stencil_view() const
        {
            return m_depth_stencil_view.get();
        }

        void setup_context(ID3D11DeviceContext& context);

        mge::rectangle default_scissor() const;

    private:
        void create_swap_chain();

        mge::dx11::render_system&                       m_render_system;
        mge::dx11::window&                              m_window;
        com_unique_ptr<ID3D11Device>                    m_device;
        com_unique_ptr<ID3D11DeviceContext>             m_device_context;
        com_unique_ptr<ID3D11RenderTargetView>          m_render_target_view;
        com_unique_ptr<ID3D11DepthStencilView>          m_depth_stencil_view;
        std::vector<std::unique_ptr<mge::index_buffer>> m_index_buffers;
    };

    inline render_context& dx11_context(mge::render_context& context)
    {
        return dynamic_cast<render_context&>(context);
    }

    inline const render_context&
    dx11_context(const mge::render_context& context)
    {
        return dynamic_cast<const render_context&>(context);
    }

} // namespace mge::dx11