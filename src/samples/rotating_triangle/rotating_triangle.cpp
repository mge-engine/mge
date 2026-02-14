// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

/**
 * @file rotating_triangle.cpp
 * @brief Sample demonstrating uniform buffer usage with animated rotation.
 *
 * This sample showcases the following MGE features:
 * - Creating and managing uniform blocks from program metadata
 * - Passing uniform data (rotation angle) from CPU to GPU
 * - Computing transformations in vertex shaders
 * - Separating update logic from rendering
 * - Cross-platform shader support (GLSL/HLSL)
 *
 * The triangle rotates at 45 degrees per second around the Z-axis.
 * Rotation is computed in the vertex shader using the angle uniform.
 */

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
#include "mge/graphics/uniform_block.hpp"
#include "mge/graphics/window.hpp"
#include "mge/math/glm.hpp"
#include "mge/math/mat.hpp"

namespace mge {
    MGE_DEFINE_TRACE(ROTATING_TRIANGLE);
}

namespace mge {
    class rotating_triangle : public application
    {
    public:
        rotating_triangle() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Setup rotating_triangle");

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

            add_update_listener(
                [&](uint64_t cycle, double elapsed, double delta) {
                    this->update_rotation(cycle, delta);
                });
            add_redraw_listener(
                [&](uint64_t cycle, double /*unused*/) { this->draw(); });
            m_window->show();
            initialize();
        }

        void screenshot()
        {
            MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Taking screenshot");
            auto       img = m_window->render_context().screenshot();
            mge::asset img_asset("/temp/screenshot.png");
            img_asset.store(mge::asset_type("image", "png"), img);
            MGE_DEBUG_TRACE(ROTATING_TRIANGLE,
                            "Screenshot saved to /temp/screenshot.png");
        }

        /**
         * @brief Update rotation angle and uniform block.
         *
         * Called each frame via update_listener. Updates the rotation angle
         * and passes it to the shader via the uniform block. Only updates
         * uniform data - does not perform any rendering.
         *
         * @param cycle Frame number
         * @param delta Time since last frame in seconds
         */
        void update_rotation(uint64_t cycle, double delta)
        {
            if (m_initialized) {
                // Create uniform block on first frame after linking completes
                if (!m_uniform_block && !m_program->needs_link()) {
                    const auto& uniform_buffers = m_program->uniform_buffers();
                    if (!uniform_buffers.empty()) {
                        MGE_DEBUG_TRACE(ROTATING_TRIANGLE,
                                        "Creating uniform block: {}",
                                        uniform_buffers[0].name);
                        m_uniform_block =
                            std::make_unique<uniform_block>(uniform_buffers[0]);
                    } else {
                        MGE_ERROR_TRACE(ROTATING_TRIANGLE,
                                        "No uniform buffers found in program");
                        set_quit();
                        return;
                    }
                }

                if (m_uniform_block) {
                    // Update rotation angle (45 degrees per second)
                    m_rotation_angle += static_cast<float>(
                        45.0 * delta); // 45 degrees per second
                    if (m_rotation_angle >= 360.0f) {
                        m_rotation_angle -= 360.0f;
                    }

                    // Update uniform block with new angle
                    // This increments the block's version counter, triggering
                    // GPU buffer update on next draw
                    m_uniform_block->set("angle", m_rotation_angle);
                }
            }
        }

        /**
         * @brief Render the rotating triangle.
         *
         * Called each frame via redraw_listener. Sets up render pass,
         * binds uniform block, submits draw commands, and presents frame.
         * Separated from update logic to follow proper update/render pattern.
         */
        void draw()
        {
            if (m_initialized && m_uniform_block) {
                auto& pass = m_window->render_context().pass(0);
                pass.default_viewport();
                pass.default_scissor();
                pass.clear_color(rgba_color(0.0f, 0.0f, 0.2f, 1.0f));
                pass.clear_depth(1.0f);

                auto& command_buffer =
                    m_window->render_context().command_buffer(true);
                command_buffer.bind_uniform_block(m_uniform_block.get());
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
            MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Initializing objects");

            auto pixel_shader =
                m_window->render_context().create_shader(shader_type::FRAGMENT);
            auto vertex_shader =
                m_window->render_context().create_shader(shader_type::VERTEX);
            m_program = m_window->render_context().create_program();
            MGE_DEBUG_TRACE(ROTATING_TRIANGLE,
                            "render system is {}",
                            m_render_system->implementation_name());

            if (m_render_system->implementation_name() ==
                    "mge::opengl::render_system" ||
                m_render_system->implementation_name() ==
                    "mge::vulkan::render_system") {
                const char* vertex_shader_glsl = R"shader(
                    #version 450 core
                    layout(location = 0) in vec3 vertexPosition;

                    layout(std140, binding = 0) uniform TransformBlock {
                        float angle;
                    };

                    void main() {
                      float rad = radians(angle);
                      float c = cos(rad);
                      float s = sin(rad);
                      mat2 rotation = mat2(c, s, -s, c);
                      vec2 rotated = rotation * vertexPosition.xy;
                      gl_Position = vec4(rotated, vertexPosition.z, 1.0);
                    }
                )shader";

                const char* fragment_shader_glsl = R"shader(
                    #version 450 core
                    layout(location = 0) out vec3 color;
                    void main() {
                        color = vec3(1,1,0);
                    }
                )shader";
                MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Compile fragment shader");
                pixel_shader->compile(fragment_shader_glsl);
                MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Compile vertex shader");
                vertex_shader->compile(vertex_shader_glsl);
                MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Shaders compiled");
            } else if (m_render_system->implementation_name() ==
                           "mge::dx11::render_system" ||
                       m_render_system->implementation_name() ==
                           "mge::dx12::render_system") {

                const char* vertex_shader_hlsl = R"shader(
                    cbuffer TransformBlock : register(b0)
                    {
                        float angle;
                    };

                    float4 main( float3 pos : POSITION ) : SV_POSITION
                    {
                        float rad = radians(angle);
                        float c = cos(rad);
                        float s = sin(rad);
                        float2x2 rotation = float2x2(c, s, -s, c);
                        float2 rotated = mul(rotation, pos.xy);
                        return float4(rotated, pos.z, 1.0);
                    }
                )shader";

                const char* fragment_shader_hlsl = R"shader(
                    float4 main() : SV_TARGET
                    {
                        return float4(1.0f, 1.0f, 0.0f, 1.0f);
                    }
                )shader";
                MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Compile fragment shader");
                pixel_shader->compile(fragment_shader_hlsl);
                MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Compile vertex shader");
                vertex_shader->compile(vertex_shader_hlsl);
                MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Shaders compiled");
            } else {
                MGE_ERROR_TRACE(ROTATING_TRIANGLE,
                                "Cannot create shaders for {} render system",
                                m_render_system->implementation_name());
                MGE_THROW(mge::illegal_state) << "Cannot create shaders";
            }
            m_program->set_shader(pixel_shader);
            m_program->set_shader(vertex_shader);
            MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Linking program");
            m_program->link();

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
            MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Create vertex buffer");
            m_vertices = m_window->render_context().create_vertex_buffer(
                layout,
                sizeof(triangle_coords),
                mge::make_buffer(triangle_coords));

            MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Create index buffer");
            m_indices = m_window->render_context().create_index_buffer(
                mge::data_type::INT32,
                sizeof(triangle_indices),
                mge::make_buffer(triangle_indices));
            MGE_DEBUG_TRACE(ROTATING_TRIANGLE, "Initializing objects done");
            m_initialized = true;
        }

    private:
        render_system_ref              m_render_system;
        window_ref                     m_window;
        std::atomic<bool>              m_initialized;
        program_handle                 m_program;
        vertex_buffer_handle           m_vertices;
        index_buffer_handle            m_indices;
        std::unique_ptr<uniform_block> m_uniform_block;
        float                          m_rotation_angle{0.0f};
    };

    MGE_REGISTER_IMPLEMENTATION(rotating_triangle,
                                mge::application,
                                rotating_triangle);
} // namespace mge

MGE_MAINFUNCTION
