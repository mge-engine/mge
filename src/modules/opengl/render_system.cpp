// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"
#include "window.hpp"
#include <memory>

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#endif
namespace mge {

    MGE_USE_TRACE(OPENGL);

    namespace opengl {
        class render_system : public mge::render_system
        {
        public:
            using mge::render_system::create_window;

            render_system()
            {
                MGE_INFO_TRACE(OPENGL) << "Creating opengl render system";
                init_capabilities();
            }
            ~render_system() = default;

            mge::window_ref create_window(const mge::extent&         extent,
                                          const mge::window_options& options)
            {
                auto ref =
                    std::make_shared<mge::opengl::window>(extent, options);
                return ref;
            }

            std::span<mge::monitor_ref> monitors() override
            {
#ifdef MGE_OS_WINDOWS
                return mge::win32::monitor::all_monitors();
#endif
            }

            class capabilities : public mge::render_system::capabilities
            {
            public:
                capabilities(render_system* system);
                ~capabilities() = default;

                std::span<const mge::shader_language>
                shader_languages() const override
                {
                    return std::span<const mge::shader_language>(
                        m_shader_languages.data(),
                        m_shader_languages.size());
                }

                std::span<const mge::shader_format>
                shader_formats() const override
                {
                    return std::span<const mge::shader_format>(
                        m_shader_formats.data(),
                        m_shader_formats.size());
                }

            private:
                std::vector<mge::shader_language> m_shader_languages;
                std::vector<mge::shader_format>   m_shader_formats;
            };

            void init_capabilities()
            {
                m_capabilities = std::make_unique<capabilities>(this);
            }
        };

        render_system::capabilities::capabilities(render_system* system)
        {
            auto            window = system->create_window();
            render_context* context =
                dynamic_cast<render_context*>(&window->render_context());

            m_shader_languages.emplace_back(context->gl_info().shader_language);
        }

        MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, opengl);
    } // namespace opengl
} // namespace mge