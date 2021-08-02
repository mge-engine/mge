#pragma once
#include "dx11.hpp"
#include "mge/graphics/render_context.hpp"
namespace mge::dx11 {

    class render_system;
    class window;

    class render_context : public mge::render_context
    {
    public:
        render_context(render_system &system, window &window_);
        virtual ~render_context();

    private:
        mge::dx11::render_system &m_render_system;
        mge::dx11::window &       m_window;
    };
} // namespace mge::dx11