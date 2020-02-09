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
#include "mge/graphics/shader.hpp"
#include "mge/graphics/pipeline.hpp"
#include "mge/core/log.hpp"
#include "mge/core/thread.hpp"
#include "mge/core/array_size.hpp"
#include "mge/core/debug_break.hpp"

using namespace mge;
using namespace std::string_view_literals;

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
        MGE_DEBUG_LOG(triangle) << "Initializing objects";
        auto pixel_shader = m_window->render_context().create_shader(shader_type::FRAGMENT);
        auto vertex_shader = m_window->render_context().create_shader(shader_type::VERTEX);
        m_pipeline = m_window->render_context().create_pipeline();
        MGE_DEBUG_LOG(triangle) << "render system is " << m_render_system->implementation_name();
        if (m_render_system->implementation_name() == "opengl::render_system"sv) {
            const char *vertex_shader_glsl =
                            "#version 330 core\n"
                            "layout(location = 0) in vec3 vertexPosition;\n"
                            "\n"
                            "void main() {\n"
                            "  gl_Position.xyz = vertexPosition;\n"
                            "  gl_Position.w = 1.0;\n"
                            "}";
            const char *fragment_shader_glsl =
                "#version 330 core\n"
                "out vec3 color;\n"
                "\n"
                "void main() {\n"
                "    color = vec3(1,1,1);\n"
                "}";
            MGE_DEBUG_LOG(triangle) << "Compile fragment shader";
            pixel_shader->compile(fragment_shader_glsl);
            MGE_DEBUG_LOG(triangle) << "Compile vertex shader";
            vertex_shader->compile(vertex_shader_glsl);
            MGE_DEBUG_LOG(triangle) << "Shaders compiled";
        }
        m_pipeline->set_shader(pixel_shader);
        m_pipeline->set_shader(vertex_shader);
        MGE_DEBUG_LOG(triangle) << "Linking pipeline";
        m_pipeline->link();
        MGE_DEBUG_LOG(triangle) << "Pipeline linked";
        float triangle_coords[] = {
            0.0f, 0.5f, 0.0f,
            0.45f, -0.5, 0.0f,
            -0.45f, -0.5f, 0.0f,
        };
        int triangle_indices[] = { 0, 1, 2};
        mge::vertex_layout layout;
        layout.push_back(mge::vertex_format(mge::data_type::FLOAT_VEC3, 1));
        m_vertices = m_window->render_context()
            .create_vertex_buffer(layout,
                mge::usage::DEFAULT,
                mge::array_size(triangle_coords),
                triangle_coords);
        m_indices = m_window->render_context()
            .create_index_buffer(mge::data_type::INT32,
                                 mge::usage::DEFAULT,
                                 mge::array_size(triangle_indices),
                                 triangle_indices);
        m_draw_commands = m_window->render_context().create_command_list();
        m_draw_commands->clear(rgba_color(0.0f, 0.0f, 1.0f, 1.0f));
        m_draw_commands->draw(mge::draw_command(m_pipeline, m_vertices, m_indices));
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
    pipeline_ref      m_pipeline;
    vertex_buffer_ref m_vertices;
    index_buffer_ref  m_indices;
};

MGE_REGISTER_IMPLEMENTATION(triangle, mge::application, triangle);

MGE_MAINFUNCTION
