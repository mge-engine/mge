// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "system_config.hpp"
#include "mge/graphics/render_context.hpp"
#include "win32/com_unique_ptr.hpp"

namespace dx11 {

    class window;

    class render_context : public mge::render_context
    {
    public:
        render_context(window *win,
                       const system_config& config);
        ~render_context();
        void flush() override;
        mge::vertex_buffer_ref create_vertex_buffer(const mge::vertex_layout& layout,
                                            mge::buffer_change_policy change_policy,
                                            mge::buffer_access cpu_access,
                                            mge::buffer_access gpu_access,
                                            size_t element_count,
                                                void *initial_data) override;
        mge::index_buffer_ref create_index_buffer(mge::data_type type,
                                                  mge::buffer_change_policy change_policy,
                                                  mge::buffer_access cpu_access,
                                                  mge::buffer_access gpu_access,
                                                  size_t element_count,
                                                  void *initial_data) override;
        mge::texture_ref create_texture(mge::texture_type type,
                                        mge::filter_function mag_filter,
                                        mge::filter_function min_filter,
                                        bool mipmap_use) override;
        mge::shader_ref create_shader(mge::shader_type type) override;
        mge::pipeline_ref create_pipeline() override;
        void shader_languages(std::vector<mge::shader_language>& languages) const override;
        ID3D11Device *device() const noexcept { return m_device.get(); }
        ID3D11DeviceContext *device_context() const noexcept { return m_device_context.get(); }
    private:
        void init_context(const system_config& config);

        window *m_window;

        COM_UNIQUE_PTR(ID3D11Device) m_device;
        COM_UNIQUE_PTR(ID3D11DeviceContext) m_device_context;
        COM_UNIQUE_PTR(ID3D11RenderTargetView) m_render_target_view;
        COM_UNIQUE_PTR(IDXGISwapChain) m_swap_chain;
    };

    inline ID3D11Device * dx11_device(const mge::render_context& context)
    {
        return static_cast<const render_context&>(context).device();
    }

    inline ID3D11DeviceContext *dx11_device_context(const mge::render_context& context)
    {
        return static_cast<const render_context&>(context).device_context();
    }
}
