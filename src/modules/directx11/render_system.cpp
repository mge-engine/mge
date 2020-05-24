// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"
#include "dx11.hpp"
#include "mge/asset/asset_not_found.hpp"
#include "mge/core/log.hpp"
#include "system_config.hpp"
#include "window.hpp"

#ifdef MGE_OS_WINDOWS
#    include "win32/monitor.hpp"
#endif

MGE_USE_LOG(DX11);

namespace dx11 {
    class render_system : public mge::render_system
    {
    public:
        render_system()
        {
            MGE_DEBUG_LOG(DX11) << "Creating DirectX 11 render system";
        }

        virtual ~render_system() = default;

        void configure(const mge::configuration &config) override
        {
            m_config.configure(config);
        }

        monitor_collection_type monitors() const override
        {
            return ::mge::platform::monitor::all_monitors();
        }

        mge::monitor_ref primary_monitor() const override
        {
            return ::mge::platform::monitor::primary_monitor();
        }

        mge::window_ref
        create_window(const mge::rectangle &     rect,
                      const mge::window_options &options) override
        {
            return std::make_shared<window>(rect, options, this, m_config);
        }

        mge::asset locate_shader(mge::shader_type type,
                                 std::string_view name) override
        {
            MGE_THROW(mge::asset_not_found)
                << "Shader '" << name << "' of type " << type << " not found";
        }

    private:
        system_config m_config;
    };

    MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, directx11,
                                dx11);
} // namespace dx11
