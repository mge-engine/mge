// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/window.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/usage.hpp"
#include "mge/core/log.hpp"
#include "mge/core/thread.hpp"
#include "mge/core/array_size.hpp"

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
        static float triangle_coords[] = {
            0.0f, 0.5f, 0.0f,
            0.45f, -0.5, 0.0f,
            -0.45f, -0.5f, 0.0f,
        };
        mge::vertex_layout layout;
        layout.push_back(mge::vertex_format(mge::data_type::FLOAT_VEC3, 1));
        m_vertices = m_window->render_context()
            .create_vertex_buffer(layout,
                mge::usage::DEFAULT,
                mge::array_size(triangle_coords),
                triangle_coords);

        m_draw_commands = m_window->render_context().create_command_list();
        m_draw_commands->clear(rgba_color(0.0f, 0.0f, 1.0f, 1.0f));
        m_draw_commands->finish();
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

    vertex_buffer_ref m_vertices;
    index_buffer_ref m_indices;
};

MGE_REGISTER_IMPLEMENTATION(triangle, mge::application, triangle);

MGE_MAINFUNCTION
