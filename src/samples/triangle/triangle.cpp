// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_source.hpp"
#include "mge/core/array_size.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/frame_command_list.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/topology.hpp"
#include "mge/graphics/window.hpp"

namespace mge {
    MGE_DEFINE_TRACE(TRIANGLE);
}

namespace mge {
    class triangle : public application
    {
    public:
        triangle() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(TRIANGLE, "Setup triangle");

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
                [&](mge::key k, mge::key_action a, mge::modifier m) {
                    if (a == mge::key_action::PRESS && k == mge::key::ESCAPE) {
                        set_quit();
                    }
                    if (a == mge::key_action::PRESS && k == mge::key::P) {
                        screenshot();
                    }
                });

            add_redraw_listener([&](uint64_t cycle, double delta) {
                this->draw(cycle, delta);
            });
            m_window->show();
            initialize();
        }

        void screenshot()
        {
            MGE_DEBUG_TRACE(TRIANGLE, "Taking screenshot");
            auto       img = m_window->render_context().screenshot();
            mge::asset img_asset("/temp/screenshot.png");
            img_asset.store(mge::asset_type("image", "png"), img);
            MGE_DEBUG_TRACE(TRIANGLE,
                            "Screenshot saved to /temp/screenshot.png");
        }

        void draw(uint64_t cycle, double delta)
        {
            if (m_initialized) {
                auto& pass = m_window->render_context().pass(0);
                pass.default_viewport();
                pass.clear_color(rgba_color(0.0f, 0.0f, 1.0f, 1.0f));
                pass.clear_depth(1.0f);

                auto& command_buffer =
                    m_window->render_context().command_buffer(true);
                command_buffer.draw(m_program, m_vertices, m_indices);
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
            MGE_DEBUG_TRACE(TRIANGLE, "Initializing objects");

            auto pixel_shader =
                m_window->render_context().create_shader(shader_type::FRAGMENT);
            auto vertex_shader =
                m_window->render_context().create_shader(shader_type::VERTEX);
            m_program = m_window->render_context().create_program();
            MGE_DEBUG_TRACE(TRIANGLE,
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
                    layout(location = 0) out vec3 color;
                    void main() {
                        color = vec3(1,1,1);
                    }
                )shader";
                MGE_DEBUG_TRACE(TRIANGLE, "Compile fragment shader");
                pixel_shader->compile(fragment_shader_glsl);
                MGE_DEBUG_TRACE(TRIANGLE, "Compile vertex shader");
                vertex_shader->compile(vertex_shader_glsl);
                MGE_DEBUG_TRACE(TRIANGLE, "Shaders compiled");
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
                MGE_DEBUG_TRACE(TRIANGLE, "Set code for fragment shader");
                pixel_shader->set_code(*pixel_shader_code);
                MGE_DEBUG_TRACE(TRIANGLE, "Set code for vertex shader");
                vertex_shader->set_code(*vertex_shader_code);
                MGE_DEBUG_TRACE(TRIANGLE, "Shaders created");
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
                MGE_DEBUG_TRACE(TRIANGLE, "Compile fragment shader");
                pixel_shader->compile(fragment_shader_hlsl);
                MGE_DEBUG_TRACE(TRIANGLE, "Compile vertex shader");
                vertex_shader->compile(vertex_shader_hlsl);
                MGE_DEBUG_TRACE(TRIANGLE, "Shaders compiled");
            } else {
                MGE_ERROR_TRACE(TRIANGLE,
                                "Cannot create shaders for {} render system",
                                m_render_system->implementation_name());
                MGE_THROW(mge::illegal_state) << "Cannot create shaders";
            }
            m_program->set_shader(pixel_shader);
            m_program->set_shader(vertex_shader);
            MGE_DEBUG_TRACE(TRIANGLE, "Linking program");
            m_program->link();
            MGE_DEBUG_TRACE(TRIANGLE,
                            "Program linked: {}",
                            m_program->needs_link() ? "needs link"
                                                    : "linked successfully");

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
            MGE_DEBUG_TRACE(TRIANGLE, "Create vertex buffer");
            m_vertices = m_window->render_context().create_vertex_buffer(
                layout,
                sizeof(triangle_coords),
                mge::make_buffer(triangle_coords));

            MGE_DEBUG_TRACE(TRIANGLE, "Create index buffer");
            m_indices = m_window->render_context().create_index_buffer(
                mge::data_type::INT32,
                sizeof(triangle_indices),
                mge::make_buffer(triangle_indices));
            MGE_DEBUG_TRACE(TRIANGLE, "Initializing objects done");
            m_initialized = true;
        }

    private:
        render_system_ref    m_render_system;
        window_ref           m_window;
        std::atomic<bool>    m_initialized;
        program_handle       m_program;
        vertex_buffer_handle m_vertices;
        index_buffer_handle  m_indices;
    };

    MGE_REGISTER_IMPLEMENTATION(triangle, mge::application, triangle);
} // namespace mge

MGE_MAINFUNCTION
