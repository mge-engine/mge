// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/window.hpp"
#include "mge/input/input_handler.hpp"
#include "mge/ui/ui.hpp"

namespace mge {
    MGE_DEFINE_TRACE(HELLO_UI);
}

namespace mge {
    class hello_ui : public application
    {
    public:
        hello_ui() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(HELLO_UI, "Setup hello_ui");

            m_render_system = render_system::create();
            m_window = m_render_system->create_window();
            m_window->set_close_listener([&] { set_quit(); });
            m_window->add_key_action_handler(
                [&](mge::key k, mge::key_action a, mge::modifier m) -> bool {
                    if (a == mge::key_action::PRESS && k == mge::key::ESCAPE) {
                        set_quit();
                        return true;
                    }
                    return false;
                });

            m_ui = std::make_shared<ui>();
            m_ui->attach(*m_window);

            add_redraw_listener([&](uint64_t cycle, double delta) {
                this->draw(cycle, delta);
            });

            m_window->show();
        }

        void draw(uint64_t cycle, double delta)
        {
            auto& context = m_window->render_context();
            auto& pass = context.pass(0);

            pass.default_viewport();
            pass.default_scissor();
            pass.clear_color(rgba_color(0.2f, 0.3f, 0.3f, 1.0f));
            pass.clear_depth(1.0f);

            m_ui->begin_frame();

            if (m_ui->begin_window("Hello Window", 50, 50, 300, 200)) {
                m_ui->label("Welcome to MGE Immediate UI!");

                if (m_ui->button("Hello World")) {
                    m_click_count++;
                    MGE_INFO_TRACE(HELLO_UI,
                                   "Button clicked! Count: {}",
                                   m_click_count);
                }

                std::string count_label =
                    "Button clicks: " + std::to_string(m_click_count);
                m_ui->label(count_label.c_str());

                m_ui->end_window();
            }

            m_ui->frame();
            m_ui->draw(pass);

            context.frame();
        }

    private:
        render_system_ref m_render_system;
        window_ref        m_window;
        ui_ref            m_ui;
        int               m_click_count = 0;
    };

    MGE_REGISTER_IMPLEMENTATION(hello_ui, mge::application, hello_ui);
} // namespace mge

MGE_MAINFUNCTION
