// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"
#include "mge/asset/asset_not_found.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/log.hpp"

#include "opengl.hpp"
#include "window.hpp"

#ifdef MGE_OS_WINDOWS
#    include "win32/monitor.hpp"
#endif

MGE_USE_LOG(OPENGL);

namespace opengl {
    class render_system : public mge::render_system
    {
    public:
        render_system()
        {
            MGE_DEBUG_LOG(OPENGL) << "Creating OpenGL render system";
        }
        virtual ~render_system() = default;

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
            mge::window_ref result =
                std::make_shared<window>(rect, options, this, m_debug);
            return result;
        }

        void configure(const mge::configuration &config) override
        {
            m_debug = config.value<bool>("debug", false);
#ifdef MGE_RELEASEWITHDEBUG
            m_debug = true;
#endif
        }

        mge::asset locate_shader(mge::shader_type type,
                                 const char *     name) override
        {
            MGE_THROW(mge::asset_not_found)
                << "Shader '" << name << "' of type " << type << " not found";
        }

    private:
        bool m_debug;
    };

    MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, opengl);
} // namespace opengl
