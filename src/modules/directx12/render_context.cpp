#include "render_context.hpp"
#include "error.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/trace.hpp"
#include "mge/win32/com_ptr.hpp"
#include "render_system.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {
    render_context::render_context(mge::dx12::render_system& render_system_,
                                   mge::dx12::window&        window_)
        : m_render_system(render_system_)
        , m_window(window_)
    {
        auto adapter = get_adapter();
    }

    mge::com_ptr<IDXGIAdapter4> render_context::get_adapter()
    {
        UINT factory_flags = 0;
        if (m_render_system.debug()) {
            factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
        }
        MGE_DEBUG_TRACE(DX12) << "Create DXGI Factory";
        mge::com_ptr<IDXGIFactory4> factory;
        auto rc = CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&factory));
        ::mge::dx12::error::check_hresult(rc,
                                          __FILE__,
                                          __LINE__,
                                          "",
                                          "CreateDXGIFactory2");
        if (m_render_system.warp()) {
            mge::com_ptr<IDXGIAdapter1> adapter1;
            mge::com_ptr<IDXGIAdapter4> adapter4;
            return adapter4;
        } else {
            mge::com_ptr<IDXGIAdapter4> adapter4;
            return adapter4;
        }
    }

    void render_context::initialize() {}

    render_context::~render_context() {}

    mge::index_buffer_ref render_context::create_index_buffer(mge::data_type dt,
                                                              size_t data_size,
                                                              void*  data)
    {
        mge::index_buffer_ref ref;
        return ref;
    }

    mge::vertex_buffer_ref render_context::create_vertex_buffer(
        const mge::vertex_layout& layout, size_t data_size, void* data)
    {
        mge::vertex_buffer_ref ref;
        return ref;
    }

    mge::shader_ref render_context::create_shader(shader_type t)
    {
        mge::shader_ref ref;
        return ref;
    }

} // namespace mge::dx12