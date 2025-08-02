// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/frame_command_list.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/swap_chain.hpp"
#include "mge/graphics/window.hpp"

MGE_DEFINE_TRACE(BLACKSCREEN);

namespace mge {
    class blackscreen : public application
    {
    public:
        blackscreen() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE_STREAM(BLACKSCREEN) << "Setup blackscreen";
            m_render_system = render_system::create();
            m_window = m_render_system->create_window();
            m_window->set_close_listener([&] {
                MGE_DEBUG_TRACE_STREAM(BLACKSCREEN) << "Close listener invoked";
                set_quit();
            });
            m_window->set_key_action_handler(
                [&](mge::key k, mge::key_action a, mge::modifier m) {
                    if (a == mge::key_action::PRESS && k == mge::key::ESCAPE) {
                        set_quit();
                    }
                });

            add_redraw_listener([&](uint64_t cycle, double delta) {
                auto clear_commands = m_window->render_context()
                                          .create_current_frame_command_list();
                clear_commands->clear(mge::rgba_color(0.0f, 0.0f, 0.0f, 1.0f));
                clear_commands->finish();
                clear_commands->execute();
                m_window->render_context().swap_chain()->present();
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