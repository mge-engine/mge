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
#include "mge/graphics/topology.hpp"
#include "mge/graphics/window.hpp"

namespace mge {
    MGE_DEFINE_TRACE(BLEND);
}

namespace mge {
    class blend : public application
    {
    public:
        blend() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(BLEND, "Setup blend");

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
            MGE_DEBUG_TRACE(BLEND, "Taking screenshot");
            auto       img = m_window->render_context().screenshot();
            mge::asset img_asset("/temp/screenshot.png");
            img_asset.store(mge::asset_type("image", "png"), img);
            MGE_DEBUG_TRACE(BLEND, "Screenshot saved to /temp/screenshot.png");
        }

        void draw(uint64_t cycle, double delta)
        {
            if (m_initialized) {
                auto& pass = m_window->render_context().pass(0);
                pass.default_viewport();
                pass.default_scissor();
                pass.clear_color(rgba_color(0.0f, 0.0f, 0.0f, 1.0f));
                pass.clear_depth(1.0f);

                auto& command_buffer =
                    m_window->render_context().command_buffer(true);

                command_buffer.depth_test_function(mge::test::ALWAYS);
                command_buffer.depth_write(false);

                // Use additive blending to make overlapping areas brighter
                command_buffer.blend_equation(blend_operation::ADD);
                command_buffer.blend_function(blend_factor::ONE,
                                              blend_factor::ONE);

                // Draw first triangle
                command_buffer.draw(m_program, m_vertices1, m_indices);

                // Draw second triangle (will blend additively)
                command_buffer.draw(m_program, m_vertices2, m_indices);

                pass.submit(command_buffer);
            } else {
                auto& pass = m_window->render_context().pass(0);
                pass.default_viewport();
                pass.clear_color(rgba_color(0.0f, 0.0f, 0.0f, 1.0f));
                pass.touch();
            }
            m_window->render_context().frame();
        }

        void initialize()
        {
            MGE_DEBUG_TRACE(BLEND, "Initializing objects");

            auto pixel_shader =
                m_window->render_context().create_shader(shader_type::FRAGMENT);
            auto vertex_shader =
                m_window->render_context().create_shader(shader_type::VERTEX);
            m_program = m_window->render_context().create_program();
            MGE_DEBUG_TRACE(BLEND,
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
                      gl_Position.y = gl_Position.y;
                      gl_Position.w = 1.0;
                    }
                )shader";

                const char* fragment_shader_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) out vec4 color;
                    void main() {
                        color = vec4(0.5, 0.5, 0.5, 0.5);
                    }
                )shader";
                MGE_DEBUG_TRACE(BLEND, "Compile fragment shader");
                pixel_shader->compile(fragment_shader_glsl);
                MGE_DEBUG_TRACE(BLEND, "Compile vertex shader");
                vertex_shader->compile(vertex_shader_glsl);
                MGE_DEBUG_TRACE(BLEND, "Shaders compiled");
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

                const char* fragment_shader_hlsl = R"shader(
                    float4 main() : SV_TARGET
                    {
                        return float4(0.5f, 0.5f, 0.5f, 1.0f);
                    }
                )shader";
                MGE_DEBUG_TRACE(BLEND, "Compile fragment shader");
                pixel_shader->compile(fragment_shader_hlsl);
                MGE_DEBUG_TRACE(BLEND, "Compile vertex shader");
                vertex_shader->compile(vertex_shader_hlsl);
                MGE_DEBUG_TRACE(BLEND, "Shaders compiled");
            } else {
                MGE_ERROR_TRACE(BLEND,
                                "Cannot create shaders for {} render system",
                                m_render_system->implementation_name());
                MGE_THROW(mge::illegal_state) << "Cannot create shaders";
            }
            m_program->set_shader(pixel_shader);
            m_program->set_shader(vertex_shader);
            MGE_DEBUG_TRACE(BLEND, "Linking program");
            m_program->link();
            MGE_DEBUG_TRACE(BLEND,
                            "Program linked: {}",
                            m_program->needs_link() ? "needs link"
                                                    : "linked successfully");

            // First triangle (left side)
            float triangle1_coords[] = {
                -0.5f,
                0.5f,
                0.0f, // top left
                -0.1f,
                0.5f,
                0.0f, // top right
                -0.3f,
                -0.5f,
                0.0f, // bottom
            };

            // Second triangle (right side, overlapping in the middle)
            float triangle2_coords[] = {
                -0.3f,
                0.5f,
                0.0f, // top left
                0.3f,
                0.5f,
                0.0f, // top right
                0.0f,
                -0.5f,
                0.0f, // bottom
            };

            int triangle_indices[] = {0, 1, 2};

            mge::vertex_layout layout;
            layout.push_back(mge::vertex_format(mge::data_type::FLOAT, 3));

            MGE_DEBUG_TRACE(BLEND, "Create vertex buffers");
            m_vertices1 = m_window->render_context().create_vertex_buffer(
                layout,
                sizeof(triangle1_coords),
                mge::make_buffer(triangle1_coords));

            m_vertices2 = m_window->render_context().create_vertex_buffer(
                layout,
                sizeof(triangle2_coords),
                mge::make_buffer(triangle2_coords));

            MGE_DEBUG_TRACE(BLEND, "Create index buffer");
            m_indices = m_window->render_context().create_index_buffer(
                mge::data_type::INT32,
                sizeof(triangle_indices),
                mge::make_buffer(triangle_indices));
            MGE_DEBUG_TRACE(BLEND, "Initializing objects done");
            m_initialized = true;
        }

    private:
        render_system_ref    m_render_system;
        window_ref           m_window;
        std::atomic<bool>    m_initialized;
        program_handle       m_program;
        vertex_buffer_handle m_vertices1;
        vertex_buffer_handle m_vertices2;
        index_buffer_handle  m_indices;
    };

    MGE_REGISTER_IMPLEMENTATION(blend, mge::application, blend);
} // namespace mge

MGE_MAINFUNCTION
