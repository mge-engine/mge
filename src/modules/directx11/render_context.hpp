// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/core/tuple_hash.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "mge/win32/com_unique_ptr.hpp"

namespace mge::dx11 {

    class render_system;
    class window;
    class program;

    class render_context : public mge::render_context
    {
    public:
        render_context(render_system& system, window& window_);
        virtual ~render_context();
        void initialize();

        mge::index_buffer* on_create_index_buffer(mge::data_type dt,
                                                  size_t data_size) override;

        mge::vertex_buffer*
        on_create_vertex_buffer(const mge::vertex_layout& layout,
                                size_t                    data_size) override;

        mge::shader*  on_create_shader(mge::shader_type t) override;
        mge::program* on_create_program() override;
        void          on_frame_present() override;

        mge::texture_ref create_texture(mge::texture_type type) override;
        mge::image_ref   screenshot() override;

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

        void render(const mge::pass& p) override;

    private:
        void              draw_geometry(mge::program*       program,
                                        mge::vertex_buffer* vb,
                                        mge::index_buffer*  ib,
                                        mge::uniform_block* ub,
                                        mge::texture*       tex,
                                        uint32_t            index_count = 0,
                                        uint32_t            index_offset = 0);
        void              bind_uniform_block(mge::dx11::program& dx11_program,
                                             mge::uniform_block& ub);
        ID3D11BlendState* blend_state(const mge::pipeline_state& state);
        ID3D11DepthStencilState*
        depth_stencil_state(const mge::pipeline_state& state);
        ID3D11RasterizerState*
             rasterizer_state(const mge::pipeline_state& state);
        void create_swap_chain();

        mge::dx11::render_system&              m_render_system;
        mge::dx11::window&                     m_window;
        com_unique_ptr<ID3D11Device>           m_device;
        com_unique_ptr<ID3D11DeviceContext>    m_device_context;
        com_unique_ptr<ID3D11RenderTargetView> m_render_target_view;

        com_unique_ptr<IDXGISwapChain>          m_swap_chain;
        com_unique_ptr<ID3D11Texture2D>         m_back_buffer;
        com_unique_ptr<ID3D11Texture2D>         m_depth_stencil_buffer;
        com_unique_ptr<ID3D11DepthStencilState> m_depth_stencil_state;
        com_unique_ptr<ID3D11DepthStencilState> m_depth_stencil_state_blend;
        com_unique_ptr<ID3D11DepthStencilView>  m_depth_stencil_view;

        using blend_state_cache_type =
            std::unordered_map<mge::pipeline_state,
                               com_unique_ptr<ID3D11BlendState>>;
        blend_state_cache_type m_blend_state_cache;

        using depth_stencil_state_cache_type =
            std::unordered_map<mge::pipeline_state,
                               com_unique_ptr<ID3D11DepthStencilState>>;
        depth_stencil_state_cache_type m_depth_stencil_state_cache;

        using rasterizer_state_cache_type =
            std::unordered_map<mge::pipeline_state,
                               com_unique_ptr<ID3D11RasterizerState>>;
        rasterizer_state_cache_type m_rasterizer_state_cache;

        std::map<mge::uniform_block*, com_unique_ptr<ID3D11Buffer>>
                                                m_constant_buffers;
        std::map<mge::uniform_block*, uint64_t> m_constant_buffer_versions;
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