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
            context.render_context.execute(m_clear_commands);
            context.render_context.flush();
        });
        m_window->show();
    }

private:
    render_system_ref m_render_system;
    window_ref        m_window;
    command_list_ref  m_clear_commands;
};

MGE_REGISTER_IMPLEMENTATION(triangle, mge::application, triangle);

MGE_MAINFUNCTION
