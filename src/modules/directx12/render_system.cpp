// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "dx12.hpp"
#include "error.hpp"
#include "mge/core/trace.hpp"
#include "mge/win32/com_ptr.hpp"
#include "window.hpp"

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#endif

namespace mge {
    MGE_USE_TRACE(DX12);

    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, directx12, debug, "Enable DirectX 12 debug mode", false);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, directx12, warp, "Enable DirectX 12 warp device", false);

} // namespace mge

namespace mge::dx12 {
    render_system::render_system()
    {
        MGE_DEBUG_TRACE(DX12) << "Creating DirectX 12 render system";
        enable_debug_layer();
    }

    mge::render_system::monitor_collection render_system::monitors()
    {
        return mge::win32::monitor::all_monitors();
    }

    mge::window_ref
    render_system::create_window(const mge::extent&         extent,
                                 const mge::window_options& options)
    {
        mge::window_ref ref = std::make_shared<window>(*this, extent, options);
        return ref;
    }

    bool render_system::debug() const
    {
        return MGE_PARAMETER(directx12, debug).get();
    }

    bool render_system::warp() const
    {
        return MGE_PARAMETER(directx12, warp).get();
    }

    void render_system::enable_debug_layer()
    {
        if (debug()) {
            MGE_DEBUG_TRACE(DX12) << "Enable debug layer";
            mge::com_ptr<ID3D12Debug> debug_interface;
            auto rc = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_interface));
            CHECK_HRESULT(rc, , D3D12GetDebugInterface);
            debug_interface->EnableDebugLayer();
            // TODO: check about GPU based validation
        }
    }

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                directx12,
                                dx12);
} // namespace mge::dx12
