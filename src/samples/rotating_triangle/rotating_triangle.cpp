// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

/**
 * @file rotating_triangle.cpp
 * @brief Sample demonstrating Slang shader compilation with
 * compile_and_link.
 *
 * Uses a single Slang source file compiled via
 * program::compile_and_link() instead of separate per-API shader
 * sources.
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
#include "mge/graphics/shader_language.hpp"
#include "mge/graphics/topology.hpp"
#include "mge/graphics/uniform.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "mge/graphics/window.hpp"
#include "mge/math/glm.hpp"
#include "mge/math/mat.hpp"

namespace mge {
    MGE_DEFINE_TRACE(rotating_triangle);
}

namespace mge {

    static const char* slang_shader_source = R"slang(
        cbuffer TransformBlock : register(b0)
        {
            float angle;
        };

        struct VertexInput
        {
            float3 position : POSITION;
        };

        struct VertexOutput
        {
            float4 position : SV_POSITION;
        };

        [shader("vertex")]
        VertexOutput vertexMain(VertexInput input)
        {
            float rad = radians(angle);
            float c = cos(rad);
            float s = sin(rad);
            float2x2 rotation = float2x2(c, s, -s, c);
            float2 rotated = mul(rotation, input.position.xy);

            VertexOutput output;
            output.position = float4(rotated, input.position.z, 1.0);
            return output;
        }

        [shader("fragment")]
        float4 fragmentMain() : SV_TARGET
        {
            return float4(1.0f, 1.0f, 0.0f, 1.0f);
        }
    )slang";

    class rotating_triangle : public application
    {
    public:
        rotating_triangle() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(rotating_triangle, "Setup rotating_triangle");

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
            m_window->add_key_action_handler(
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
            MGE_DEBUG_TRACE(rotating_triangle, "Taking screenshot");
            auto       img = m_window->render_context().screenshot();
            mge::asset img_asset("/temp/screenshot.png");
            img_asset.store(mge::asset_type("image", "png"), img);
            MGE_DEBUG_TRACE(rotating_triangle,
                            "Screenshot saved to /temp/screenshot.png");
        }

        void update_rotation(uint64_t cycle, double delta)
        {
            if (m_initialized) {
                if (!m_uniform_block && !m_program->needs_link()) {
                    const auto& uniform_buffers = m_program->uniform_buffers();
                    if (!uniform_buffers.empty()) {
                        MGE_DEBUG_TRACE(rotating_triangle,
                                        "Creating uniform block: {}",
                                        uniform_buffers[0].name);
                        m_uniform_block =
                            std::make_unique<uniform_block>(uniform_buffers[0]);
                    } else {
                        MGE_ERROR_TRACE(rotating_triangle,
                                        "No uniform buffers found in program");
                        set_quit();
                        return;
                    }
                }

                if (m_uniform_block) {
                    m_rotation_angle += static_cast<float>(45.0 * delta);
                    if (m_rotation_angle >= 360.0f) {
                        m_rotation_angle -= 360.0f;
                    }
                    m_angle_uniform = m_rotation_angle;
                }
            }
        }

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
            MGE_DEBUG_TRACE(rotating_triangle, "Initializing objects");

            m_program = m_window->render_context().create_program();

            mge::shader_language slang{"slang", mge::semantic_version(1, 0)};

            const auto& caps = m_render_system->system_capabilities();
            bool        slang_supported = false;
            for (const auto& lang : caps.shader_languages()) {
                if (lang.name() == "slang") {
                    slang_supported = true;
                    break;
                }
            }
            if (!slang_supported) {
                MGE_ERROR_TRACE(
                    rotating_triangle,
                    "Render system '{}' does not support Slang shaders",
                    m_render_system->implementation_name());
                set_quit();
                return;
            }

            MGE_DEBUG_TRACE(rotating_triangle,
                            "Compiling and linking Slang program");
            m_program->compile_and_link(slang, slang_shader_source);
            MGE_DEBUG_TRACE(rotating_triangle,
                            "Slang program compiled and linked");

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
            MGE_DEBUG_TRACE(rotating_triangle, "Create vertex buffer");
            m_vertices = m_window->render_context().create_vertex_buffer(
                layout,
                sizeof(triangle_coords),
                mge::make_buffer(triangle_coords));

            MGE_DEBUG_TRACE(rotating_triangle, "Create index buffer");
            m_indices = m_window->render_context().create_index_buffer(
                mge::data_type::INT32,
                sizeof(triangle_indices),
                mge::make_buffer(triangle_indices));
            MGE_DEBUG_TRACE(rotating_triangle, "Initializing objects done");
            m_initialized = true;
        }

    private:
        render_system_ref              m_render_system;
        window_ref                     m_window;
        std::atomic<bool>              m_initialized{false};
        program_handle                 m_program;
        vertex_buffer_handle           m_vertices;
        index_buffer_handle            m_indices;
        std::unique_ptr<uniform_block> m_uniform_block;
        float                          m_rotation_angle{0.0f};
        uniform<float>                 m_angle_uniform{"angle"};
    };

    MGE_REGISTER_IMPLEMENTATION(rotating_triangle,
                                mge::application,
                                rotating_triangle);
} // namespace mge

MGE_MAINFUNCTION
