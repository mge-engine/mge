// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "dx12.hpp"
#include "mge/core/trace.hpp"

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#endif

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {
    render_system::render_system()
    {
        MGE_DEBUG_TRACE(DX12) << "Creating DirectX 12 render system";
    }

    mge::render_system::monitor_collection render_system::monitors()
    {
        return mge::win32::monitor::all_monitors();
    }

    mge::window_ref
    render_system::create_window(const mge::extent&         extent,
                                 const mge::window_options& options)
    {
        mge::window_ref ref;
        return ref;
    }

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                directx12,
                                dx12);
} // namespace mge::dx12
