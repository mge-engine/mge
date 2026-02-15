// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/window.hpp"

namespace mge {
    MGE_DEFINE_TRACE(BLACKSCREEN);
}
namespace mge {
    class blackscreen : public application
    {
    public:
        blackscreen() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(BLACKSCREEN, "Setup blackscreen");
            m_render_system = render_system::create();
            m_window = m_render_system->create_window();
            m_window->set_close_listener([&] {
                MGE_DEBUG_TRACE(BLACKSCREEN, "Close listener invoked");
                set_quit();
            });
            m_window->set_key_action_handler(
                [&](mge::key k, mge::key_action a, mge::modifier m) -> bool {
                    if (a == mge::key_action::PRESS && k == mge::key::ESCAPE) {
                        set_quit();
                        return true;
                    }
                    return false;
                });

            add_redraw_listener([&](uint64_t cycle, double delta) {
                auto& pass = m_window->render_context().pass(0);
                pass.default_viewport();
                pass.clear_color(mge::rgba_color(0.0f, 0.0f, 0.0f, 1.0f));
                pass.touch();

                m_window->render_context().frame();
            });

            m_window->show();
        }

        void teardown() override
        {
            m_window.reset();
            m_render_system.reset();
        }

    private:
        render_system_ref m_render_system;
        window_ref        m_window;
    };

    MGE_REGISTER_IMPLEMENTATION(blackscreen, mge::application, blackscreen);
} // namespace mge

MGE_MAINFUNCTION