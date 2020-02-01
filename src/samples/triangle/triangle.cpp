// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/window.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/core/log.hpp"
#include "mge/core/thread.hpp"

using namespace mge;

MGE_DEFINE_LOG(triangle);

class triangle : public application
{
public:
    triangle() = default;

    void setup() override
    {
        MGE_DEBUG_LOG(triangle) << "Setup triangle";
        m_render_system = render_system::create();
        m_window = m_render_system->create_window();
        m_window->set_close_listener([&]{
            set_quit();
        });
        m_window->set_key_action_handler([&](mge::key k, mge::key_action a, mge::modifier m) {
            if(a == mge::key_action::PRESS && k == mge::key::ESCAPE) {
                set_quit();
            }
        });

        m_clear_commands = m_window->render_context().create_command_list();
        m_clear_commands->clear(rgba_color(0.0f, 0.0f, 0.0f, 1.0f));
        m_clear_commands->finish();
        m_window->set_redraw_listener([&](window::redraw_context& context) {
            this->draw(context);
        });
        MGE_DEBUG_LOG(triangle) << "Initializing objects";
        m_window->show();
        add_setup_task([&]{
            this->initialize();
        });
    }

    void initialize()
    {
        using namespace std::chrono_literals;
        MGE_DEBUG_LOG(triangle) << "Initializing objects";
        m_draw_commands = m_window->render_context().create_command_list();
        m_draw_commands->clear(rgba_color(0.0f, 0.0f, 1.0f, 1.0f));
        m_draw_commands->finish();
        mge::this_thread::sleep_for(3s);
        MGE_DEBUG_LOG(triangle) << "Initializing objects done";
        m_initialized = true;
    }

    void draw(window::redraw_context& context)
    {
        if (m_initialized) {
            context.render_context.execute(m_draw_commands);
        } else {
            context.render_context.execute(m_clear_commands);
        }
        context.render_context.flush();

    }
private:
    render_system_ref m_render_system;
    window_ref        m_window;
    std::atomic<bool> m_initialized;
    command_list_ref  m_clear_commands;
    command_list_ref  m_draw_commands;
};

MGE_REGISTER_IMPLEMENTATION(triangle, mge::application, triangle);

MGE_MAINFUNCTION
