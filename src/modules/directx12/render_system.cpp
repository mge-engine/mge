// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "dx12.hpp"
#include "system_config.hpp"
//#include "window.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/core/log.hpp"

#include "win32/monitor.hpp"
#include "win32/com_unique_ptr.hpp"


MGE_USE_LOG(DX12);


namespace dx12 {

    class render_system : public mge::render_system
    {
    public:
        render_system()
        {
            enable_debug_layer();
        }

        ~render_system()
        {}

        void configure(const mge::configuration& config)
        {
            m_config.configure(config);
        }

        monitor_collection_t monitors() const override
        {
            return ::platform::monitor::all_monitors();
        }

        mge::monitor_ref primary_monitor() const override
        {
            return platform::monitor::primary_monitor();
        }

        mge::window_ref create_window(const mge::rectangle& rect,
                                      const mge::window_options& options) override
        {
            mge::window_ref result;// = std::make_shared<window>(rect, options, m_config);
            return result;
        }

    private:
        void enable_debug_layer()
        {
#ifndef MGE_RELEASE
            ID3D12Debug *debug_controller;
            MGE_DEBUG_LOG(DX12) << "Try to enable debug layer" << std::endl;
            HRESULT rc = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller));
            if (SUCCEEDED(rc)) {
                m_debug_controller.reset(debug_controller);
                m_debug_controller->EnableDebugLayer();
                MGE_DEBUG_LOG(DX12) << "Debug layer enabled" << std::endl;
            } else {
                MGE_ERROR_LOG(DX12) << "Failed to enable debug layer: " << rc << std::endl;
            }
#endif
        }

        COM_UNIQUE_PTR(ID3D12Debug) m_debug_controller;
        system_config m_config;
    };

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                directx12,dx12);


}


