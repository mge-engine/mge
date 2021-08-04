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
        render_context(render_system &system, window &window_);
        virtual ~render_context();

    private:
        void init_context();

        mge::dx11::render_system &m_render_system;
        mge::dx11::window &       m_window;

        mge::com_unique_ptr<ID3D11Device>        m_device;
        mge::com_unique_ptr<ID3D11DeviceContext> m_device_context;
    };
} // namespace mge::dx11