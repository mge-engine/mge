// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
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

        mge::index_buffer_ref
        create_index_buffer(mge::data_type dt, size_t element_count, void* initial_data) override;

        const ::mge::dx11::window& window() const { return m_window; }

        const ::mge::dx11::render_system& render_system() const { return m_render_system; }

        ID3D11Device* device() const { return m_device.get(); }

    private:
        void init_context();
        void create_swap_chain();

        mge::dx11::render_system& m_render_system;
        mge::dx11::window&        m_window;

        com_unique_ptr<ID3D11Device>           m_device;
        com_unique_ptr<ID3D11DeviceContext>    m_device_context;
        com_unique_ptr<ID3D11RenderTargetView> m_render_target_view;
    };
} // namespace mge::dx11