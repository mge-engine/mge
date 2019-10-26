// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "dx12.hpp"
#include "system_config.hpp"
#include "window.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/core/log.hpp"

#include "win32/monitor.hpp"
#include "win32/com_unique_ptr.hpp"
#include "win32/com_ptr.hpp"

MGE_USE_LOG(DX12);

namespace dx12 {

    class render_system : public mge::render_system
    {
    public:
        render_system()
        {}

        ~render_system() override
        {}

        void configure(const mge::configuration& config) override
        {
            m_config.configure(config);
            if (m_config.debug()) {
                enable_debug_layer();
            }
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
            return std::make_shared<window>(rect,
                                            options,
                                            m_config);
        }

    private:
        void enable_debug_layer()
        {
            mge::com_ptr<ID3D12Debug> debug_controller;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)))) {
                MGE_DEBUG_LOG(DX12) << "Enable debug layer";
                debug_controller->EnableDebugLayer();
            }
        }
        system_config m_config;
    };

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                directx12, dx12);


}


