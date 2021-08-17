// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "dx11.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/trace.hpp"
#include "window.hpp"

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#endif

namespace mge {
    MGE_USE_TRACE(DX11);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, directx11, debug, "Enable DirectX11 debug mode", false);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(bool,
                                      directx11,
                                      software_device,
                                      "Use the DirextX11 software device",
                                      false);
} // namespace mge

namespace mge::dx11 {
    render_system::render_system()
    {
        MGE_DEBUG_TRACE(DX11) << "Creating DirectX 11 render system";
    }

    render_system::monitor_collection render_system::monitors()
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
        return MGE_PARAMETER(directx11, debug).get();
    }

    bool render_system::software_device() const
    {
        return MGE_PARAMETER(directx11, software_device).get();
    }

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                directx11,
                                dx11);
} // namespace mge::dx11
