// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/asset/asset.hpp"
#include "mge/core/array_size.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/frame_command_list.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/swap_chain.hpp"
#include "mge/graphics/topology.hpp"
#include "mge/graphics/window.hpp"

MGE_DEFINE_TRACE(TRIANGLE);

namespace mge {
    class triangle : public application
    {
    public:
        triangle() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Setup triangle";

            mge::properties p;
            p.set("directory", "./assets");
            mge::asset::mount("/", "file", p);

            m_render_system = render_system::create();
            m_window = m_render_system->create_window();
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

            add_redraw_listener([&](uint64_t cycle, double delta) {
                this->draw(cycle, delta);
            });
            m_window->show();
            initialize();
        }

        void draw(uint64_t cycle, double delta)
        {
            if (m_initialized) {
                auto draw_commands = m_window->render_context()
                                         .create_current_frame_command_list();
                draw_commands->clear(rgba_color(0.0f, 0.0f, 1.0f, 1.0f));
                draw_commands->default_scissor();
                draw_commands->draw(
                    mge::draw_command(m_program,
                                      m_vertices,
                                      m_indices,
                                      mge::topology::TRIANGLES));
                draw_commands->finish();
                draw_commands->execute();
            }
            m_window->render_context().swap_chain()->present();
        }

        void initialize()
        {
            MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Initializing objects";

            auto pixel_shader =
                m_window->render_context().create_shader(shader_type::FRAGMENT);
            auto vertex_shader =
                m_window->render_context().create_shader(shader_type::VERTEX);
            m_program = m_window->render_context().create_program();
            MGE_DEBUG_TRACE_STREAM(TRIANGLE)
                << "render system is "
                << m_render_system->implementation_name();

            if (m_render_system->implementation_name() ==
                    "mge::opengl::render_system" ||
                m_render_system->implementation_name() ==
                    "mge::vulkan::render_system") {
                const char* vertex_shader_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) in vec3 vertexPosition;

                    void main() {
                      gl_Position.xyz = vertexPosition;
                      gl_Position.y = gl_Position.y;
                      gl_Position.w = 1.0;
                    }
                )shader";

                const char* fragment_shader_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) out vec3 color;
                    void main() {
                        color = vec3(1,1,1);
                    }
                )shader";
                MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Compile fragment shader";
                pixel_shader->compile(fragment_shader_glsl);
                MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Compile vertex shader";
                vertex_shader->compile(vertex_shader_glsl);
                MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Shaders compiled";
            } else if (m_render_system->implementation_name() ==
                       "mge::vulkan::render_system") {
                auto pixel_shader_code_any =
                    mge::asset("/shaders/triangle.frag.spv").load();
                auto vertex_shader_code_any =
                    mge::asset("/shaders/triangle.vert.spv").load();
                auto pixel_shader_code =
                    std::any_cast<std::shared_ptr<mge::buffer>>(
                        pixel_shader_code_any);
                auto vertex_shader_code =
                    std::any_cast<std::shared_ptr<mge::buffer>>(
                        vertex_shader_code_any);
                MGE_DEBUG_TRACE_STREAM(TRIANGLE)
                    << "Set code for fragment shader";
                pixel_shader->set_code(*pixel_shader_code);
                MGE_DEBUG_TRACE_STREAM(TRIANGLE)
                    << "Set code for vertex shader";
                vertex_shader->set_code(*vertex_shader_code);
                MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Shaders created";
            } else if (m_render_system->implementation_name() ==
                           "mge::dx11::render_system" ||
                       m_render_system->implementation_name() ==
                           "mge::dx12::render_system") {

                const char* vertex_shader_hlsl = R"shader(
                    float4 main( float4 pos : POSITION ) : SV_POSITION
                    {
                        return pos;
                    }
                )shader";

                const char* fragment_shader_hlsl = R"shader(
                    float4 main() : SV_TARGET
                    {
                        return float4(1.0f, 1.0f, 1.0f, 1.0f);
                    }
                )shader";
                MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Compile fragment shader";
                pixel_shader->compile(fragment_shader_hlsl);
                MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Compile vertex shader";
                vertex_shader->compile(vertex_shader_hlsl);
                MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Shaders compiled";
            } else {
                MGE_ERROR_TRACE(TRIANGLE)
                    << "Cannot create shaders for "
                    << m_render_system->implementation_name()
                    << " render system";
                MGE_THROW(mge::illegal_state) << "Cannot create shaders";
            }
            m_program->set_shader(pixel_shader);
            m_program->set_shader(vertex_shader);
            MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Linking program";
            m_program->link();
            MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Program linked";

            float triangle_coords[] = {
                0.0f,
                0.5f,
                0.0f,
                0.45f,
                -0.5,
                0.0f,
                -0.45f,
                -0.5f,
                0.0f,
            };
            int                triangle_indices[] = {0, 1, 2};
            mge::vertex_layout layout;
            layout.push_back(mge::vertex_format(mge::data_type::FLOAT, 3));
            MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Create vertex buffer";
            m_vertices = m_window->render_context().create_vertex_buffer(
                layout,
                sizeof(triangle_coords),
                triangle_coords);
            MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Create index buffer";
            m_indices = m_window->render_context().create_index_buffer(
                mge::data_type::INT32,
                sizeof(triangle_indices),
                triangle_indices);
            m_draw_commands = m_window->render_context().create_command_list();
            m_draw_commands->clear(rgba_color(0.0f, 0.0f, 1.0f, 1.0f));
            m_draw_commands->draw(mge::draw_command(m_program,
                                                    m_vertices,
                                                    m_indices,
                                                    mge::topology::TRIANGLES));
            m_draw_commands->finish();
            MGE_DEBUG_TRACE_STREAM(TRIANGLE) << "Initializing objects done";

            m_initialized = true;
        }

    private:
        render_system_ref m_render_system;
        window_ref        m_window;
        std::atomic<bool> m_initialized;
        command_list_ref  m_clear_commands;
        command_list_ref  m_draw_commands;
        program_ref       m_program;
        vertex_buffer_ref m_vertices;
        index_buffer_ref  m_indices;
    };

    MGE_REGISTER_IMPLEMENTATION(triangle, mge::application, triangle);
} // namespace mge

MGE_MAINFUNCTION
