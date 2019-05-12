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
        mge::index_buffer_ref create_index_buffer(mge::buffer_usage usage,
                                                  mge::buffer_access cpu_access,
                                                  mge::buffer_access gpu_access) override;
        mge::index_buffer_ref create_index_buffer(mge::buffer_usage usage,
                                                  mge::buffer_access cpu_access,
                                                  mge::buffer_access gpu_access,
                                                  void *data,
                                                  size_t size) override;
        mge::shader_ref create_shader(mge::shader_type type) override;
        void shader_languages(std::vector<mge::shader_language>& languages) const override;
    private:
        void init_context(const system_config& config);

        window *m_window;

        COM_UNIQUE_PTR(ID3D11Device) m_device;
        COM_UNIQUE_PTR(ID3D11DeviceContext) m_device_context;
        COM_UNIQUE_PTR(ID3D11RenderTargetView) m_render_target_view;
        COM_UNIQUE_PTR(IDXGISwapChain) m_swap_chain;
    };
}
