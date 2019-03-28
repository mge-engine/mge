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
    private:
        void init_context(const system_config& config);

        window *m_window;

        COM_UNIQUE_PTR(ID3D11Device) m_device;
        COM_UNIQUE_PTR(ID3D11DeviceContext) m_device_context;
        COM_UNIQUE_PTR(ID3D11RenderTargetView) m_render_target_view;
        COM_UNIQUE_PTR(IDXGISwapChain) m_swap_chain;
    };
}
