// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_source.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/command_buffer.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/test.hpp"
#include "mge/graphics/topology.hpp"
#include "mge/graphics/window.hpp"

namespace mge {
    MGE_DEFINE_TRACE(DEPTH_TEST);
}

namespace mge {
    class depth_test : public application
    {
    public:
        depth_test() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(DEPTH_TEST, "Setup depth_test");

            mge::properties p;
            p.set("directory", "./assets");
            mge::asset::mount("/", "file", p);
            p.set("directory", "./temp");
            mge::asset::mount("/temp",
                              "file",
                              mge::asset_source::access_mode::READ_WRITE,
                              p);

            m_render_system = render_system::create();
            m_window = m_render_system->create_window();
            m_window->set_close_listener([&] { set_quit(); });
            m_window->set_key_action_handler(
                [&](mge::key k, mge::key_action a, mge::modifier m) -> bool {
                    if (a == mge::key_action::PRESS && k == mge::key::ESCAPE) {
                        set_quit();
                        return true;
                    }
                    if (a == mge::key_action::PRESS && k == mge::key::P) {
                        screenshot();
                        return true;
                    }
                    return false;
                });

            add_redraw_listener([&](uint64_t cycle, double delta) {
                this->draw(cycle, delta);
            });
            m_window->show();
            initialize();
        }

        void screenshot()
        {
            MGE_DEBUG_TRACE(DEPTH_TEST, "Taking screenshot");
            auto       img = m_window->render_context().screenshot();
            mge::asset img_asset("/temp/screenshot.png");
            img_asset.store(mge::asset_type("image", "png"), img);
            MGE_DEBUG_TRACE(DEPTH_TEST,
                            "Screenshot saved to /temp/screenshot.png");
        }

        void draw(uint64_t cycle, double delta)
        {
            if (m_initialized) {
                auto& pass = m_window->render_context().pass(0);
                pass.default_viewport();
                pass.default_scissor();
                pass.clear_color(rgba_color(0.2f, 0.2f, 0.2f, 1.0f));
                pass.clear_depth(1.0f);

                auto& command_buffer =
                    m_window->render_context().command_buffer(true);

                // Use default depth test (LESS) - nearer objects occlude
                // farther ones

                // Draw back triangle (red, at z=0.5)
                command_buffer.draw(m_program_back, m_vertices_back, m_indices);

                // Draw front triangle (green, at z=0.0)
                command_buffer.draw(m_program_front,
                                    m_vertices_front,
                                    m_indices);

                pass.submit(command_buffer);
            } else {
                auto& pass = m_window->render_context().pass(0);
                pass.default_viewport();
                pass.clear_color(rgba_color(0.2f, 0.2f, 0.2f, 1.0f));
                pass.touch();
            }
            m_window->render_context().frame();
        }

        void initialize()
        {
            MGE_DEBUG_TRACE(DEPTH_TEST, "Initializing objects");

            m_program_back = m_window->render_context().create_program();
            m_program_front = m_window->render_context().create_program();

            MGE_DEBUG_TRACE(DEPTH_TEST,
                            "render system is {}",
                            m_render_system->implementation_name());

            if (m_render_system->implementation_name() ==
                    "mge::opengl::render_system" ||
                m_render_system->implementation_name() ==
                    "mge::vulkan::render_system") {
                const char* vertex_shader_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) in vec3 vertexPosition;

                    void main() {
                      gl_Position.xyz = vertexPosition;
                      gl_Position.w = 1.0;
                    }
                )shader";

                const char* fragment_shader_red_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) out vec4 color;
                    void main() {
                        color = vec4(1.0, 0.0, 0.0, 1.0);
                    }
                )shader";

                const char* fragment_shader_green_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) out vec4 color;
                    void main() {
                        color = vec4(0.0, 1.0, 0.0, 1.0);
                    }
                )shader";

                auto vertex_shader = m_window->render_context().create_shader(
                    shader_type::VERTEX);
                auto pixel_shader_red =
                    m_window->render_context().create_shader(
                        shader_type::FRAGMENT);
                auto pixel_shader_green =
                    m_window->render_context().create_shader(
                        shader_type::FRAGMENT);

                MGE_DEBUG_TRACE(DEPTH_TEST, "Compile shaders");
                vertex_shader->compile(vertex_shader_glsl);
                pixel_shader_red->compile(fragment_shader_red_glsl);
                pixel_shader_green->compile(fragment_shader_green_glsl);

                m_program_back->set_shader(vertex_shader);
                m_program_back->set_shader(pixel_shader_red);
                m_program_front->set_shader(vertex_shader);
                m_program_front->set_shader(pixel_shader_green);

            } else if (m_render_system->implementation_name() ==
                           "mge::dx11::render_system" ||
                       m_render_system->implementation_name() ==
                           "mge::dx12::render_system") {
                const char* vertex_shader_hlsl = R"shader(
                    float4 main( float3 pos : POSITION ) : SV_POSITION
                    {
                        return float4(pos, 1.0);
                    }
                )shader";

                const char* fragment_shader_red_hlsl = R"shader(
                    float4 main() : SV_TARGET
                    {
                        return float4(1.0f, 0.0f, 0.0f, 1.0f);
                    }
                )shader";

                const char* fragment_shader_green_hlsl = R"shader(
                    float4 main() : SV_TARGET
                    {
                        return float4(0.0f, 1.0f, 0.0f, 1.0f);
                    }
                )shader";

                auto vertex_shader = m_window->render_context().create_shader(
                    shader_type::VERTEX);
                auto pixel_shader_red =
                    m_window->render_context().create_shader(
                        shader_type::FRAGMENT);
                auto pixel_shader_green =
                    m_window->render_context().create_shader(
                        shader_type::FRAGMENT);

                MGE_DEBUG_TRACE(DEPTH_TEST, "Compile shaders");
                vertex_shader->compile(vertex_shader_hlsl);
                pixel_shader_red->compile(fragment_shader_red_hlsl);
                pixel_shader_green->compile(fragment_shader_green_hlsl);

                m_program_back->set_shader(vertex_shader);
                m_program_back->set_shader(pixel_shader_red);
                m_program_front->set_shader(vertex_shader);
                m_program_front->set_shader(pixel_shader_green);
            } else {
                MGE_ERROR_TRACE(DEPTH_TEST,
                                "Cannot create shaders for {} render system",
                                m_render_system->implementation_name());
                MGE_THROW(mge::illegal_state) << "Cannot create shaders";
            }

            MGE_DEBUG_TRACE(DEPTH_TEST, "Linking programs");
            m_program_back->link();
            m_program_front->link();

            // Back triangle (red) at z=0.5 (farther from camera)
            float back_coords[] = {
                -0.5f,
                0.5f,
                0.5f, // top left
                0.5f,
                0.5f,
                0.5f, // top right
                0.0f,
                -0.5f,
                0.5f, // bottom
            };

            // Front triangle (green) at z=0.0 (closer to camera)
            // Overlaps the bottom half of the back triangle
            float front_coords[] = {
                -0.3f,
                0.0f,
                0.0f, // top left
                0.3f,
                0.0f,
                0.0f, // top right
                0.0f,
                -0.8f,
                0.0f, // bottom
            };

            int triangle_indices[] = {0, 1, 2};

            mge::vertex_layout layout;
            layout.push_back(mge::vertex_format(mge::data_type::FLOAT, 3));

            MGE_DEBUG_TRACE(DEPTH_TEST, "Create vertex buffers");
            m_vertices_back = m_window->render_context().create_vertex_buffer(
                layout,
                sizeof(back_coords),
                mge::make_buffer(back_coords));

            m_vertices_front = m_window->render_context().create_vertex_buffer(
                layout,
                sizeof(front_coords),
                mge::make_buffer(front_coords));

            MGE_DEBUG_TRACE(DEPTH_TEST, "Create index buffer");
            m_indices = m_window->render_context().create_index_buffer(
                mge::data_type::INT32,
                sizeof(triangle_indices),
                mge::make_buffer(triangle_indices));
            MGE_DEBUG_TRACE(DEPTH_TEST, "Initializing objects done");
            m_initialized = true;
        }

    private:
        render_system_ref    m_render_system;
        window_ref           m_window;
        std::atomic<bool>    m_initialized;
        program_handle       m_program_back;
        program_handle       m_program_front;
        vertex_buffer_handle m_vertices_back;
        vertex_buffer_handle m_vertices_front;
        index_buffer_handle  m_indices;
    };

    MGE_REGISTER_IMPLEMENTATION(depth_test, mge::application, depth_test);
} // namespace mge

MGE_MAINFUNCTION
