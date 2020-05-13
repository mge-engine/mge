// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/array_size.hpp"
#include "mge/core/debug_break.hpp"
#include "mge/core/log.hpp"
#include "mge/core/thread.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/pipeline.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/shader_program.hpp"
#include "mge/graphics/usage.hpp"
#include "mge/graphics/window.hpp"

using namespace mge;
using namespace std::string_view_literals;

MGE_DEFINE_LOG(cube);

class cube : public application
{
public:
    cube() = default;

    void setup() override
    {
        MGE_DEBUG_LOG(cube) << "Setup cube";
        m_render_system = render_system::create();
        m_window        = m_render_system->create_window();
        m_window->set_close_listener([&] { set_quit(); });
        m_window->set_key_action_handler(
            [&](mge::key k, mge::key_action a, mge::modifier m) {
                if (a == mge::key_action::PRESS && k == mge::key::ESCAPE) {
                    set_quit();
                }
            });

        m_clear_commands = m_window->render_context().create_command_list();
        m_clear_commands->clear(rgba_color(0.0f, 0.0f, 0.0f, 1.0f));
        m_clear_commands->finish();
        m_window->set_redraw_listener(
            [&](window::redraw_context &context) { this->draw(context); });
        MGE_DEBUG_LOG(cube) << "Initializing objects";
        m_window->show();
        add_setup_task([&] { this->initialize(); });
    }

    void initialize()
    {
        MGE_DEBUG_LOG(cube) << "Initializing objects";
        auto pixel_shader = m_window->render_context().create_shader_program(
            shader_type::FRAGMENT);
        auto vertex_shader = m_window->render_context().create_shader_program(
            shader_type::VERTEX);
        m_pipeline = m_window->render_context().create_pipeline();
        MGE_DEBUG_LOG(cube)
            << "render system is " << m_render_system->implementation_name();
        if (m_render_system->implementation_name() ==
            "opengl::render_system"sv) {
            const char *vertex_shader_glsl =
                "#version 330 core\n"
                "layout(location = 0) in vec3 vertexPosition;\n"
                "\n"
                "void main() {\n"
                "  gl_Position.xyz = vertexPosition;\n"
                "  gl_Position.w = 1.0;\n"
                "}";
            const char *fragment_shader_glsl = "#version 330 core\n"
                                               "out vec3 color;\n"
                                               "\n"
                                               "void main() {\n"
                                               "    color = vec3(1,1,1);\n"
                                               "}";
            MGE_DEBUG_LOG(cube) << "Compile fragment shader";
            pixel_shader->compile(fragment_shader_glsl);
            MGE_DEBUG_LOG(cube) << "Compile vertex shader";
            vertex_shader->compile(vertex_shader_glsl);
            MGE_DEBUG_LOG(cube) << "Shaders compiled";
        }
        m_pipeline->set_shader_program(pixel_shader);
        m_pipeline->set_shader_program(vertex_shader);
        MGE_DEBUG_LOG(cube) << "Linking pipeline";
        m_pipeline->link();
        MGE_DEBUG_LOG(cube) << "Pipeline linked";
        float cube_coords[] = {
            -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
            1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f, -1.0f, 1.0f,
            1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        };
        int cube_indices[] = {
            0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6,
            1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
        };
        mge::vertex_layout layout;
        layout.push_back(mge::vertex_format(mge::data_type::FLOAT_VEC3, 1));
        m_vertices = m_window->render_context().create_vertex_buffer(
            layout, mge::usage::DEFAULT, mge::array_size(cube_coords),
            cube_coords);
        m_indices = m_window->render_context().create_index_buffer(
            mge::data_type::INT32, mge::usage::DEFAULT,
            mge::array_size(cube_indices), cube_indices);
        m_draw_commands = m_window->render_context().create_command_list();
        m_draw_commands->clear(rgba_color(0.0f, 0.0f, 1.0f, 1.0f));
        m_draw_commands->draw(
            mge::draw_command(m_pipeline, m_vertices, m_indices));
        m_draw_commands->finish();
        MGE_DEBUG_LOG(cube) << "Initializing objects done";
        m_initialized = true;
    }

    void draw(window::redraw_context &context)
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
    pipeline_ref      m_pipeline;
    vertex_buffer_ref m_vertices;
    index_buffer_ref  m_indices;
};

MGE_REGISTER_IMPLEMENTATION(cube, mge::application, cube);

MGE_MAINFUNCTION
