// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

/**
 * @file textured_triangle.cpp
 * @brief Sample demonstrating texture mapping on a triangle.
 *
 * This sample loads the sand.png image as a texture and maps it
 * onto a triangle using UV coordinates.
 */

#include "mge/application/application.hpp"
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_source.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/command_buffer.hpp"
#include "mge/graphics/image.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/texture.hpp"
#include "mge/graphics/texture_type.hpp"
#include "mge/graphics/topology.hpp"
#include "mge/graphics/window.hpp"

namespace mge {
    MGE_DEFINE_TRACE(TEXTURED_TRIANGLE);
}

namespace mge {
    class textured_triangle : public application
    {
    public:
        textured_triangle() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Setup textured_triangle");

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
            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Taking screenshot");
            auto       img = m_window->render_context().screenshot();
            mge::asset img_asset("/temp/screenshot.png");
            img_asset.store(mge::asset_type("image", "png"), img);
            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE,
                            "Screenshot saved to /temp/screenshot.png");
        }

        void draw(uint64_t cycle, double delta)
        {
            if (m_initialized) {
                auto& pass = m_window->render_context().pass(0);
                pass.default_viewport();
                pass.default_scissor();
                pass.clear_color(rgba_color(0.0f, 0.0f, 0.2f, 1.0f));
                pass.clear_depth(1.0f);

                auto& command_buffer =
                    m_window->render_context().command_buffer(true);
                command_buffer.bind_texture(m_texture.get());
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
            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Initializing objects");

            auto& ctx = m_window->render_context();

            auto pixel_shader = ctx.create_shader(shader_type::FRAGMENT);
            auto vertex_shader = ctx.create_shader(shader_type::VERTEX);
            m_program = ctx.create_program();
            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE,
                            "render system is {}",
                            m_render_system->implementation_name());

            if (m_render_system->implementation_name() ==
                    "mge::opengl::render_system" ||
                m_render_system->implementation_name() ==
                    "mge::vulkan::render_system") {

                const char* vertex_shader_glsl = R"shader(
                    #version 450 core
                    layout(location = 0) in vec3 vertexPosition;
                    layout(location = 1) in vec2 vertexTexCoord;

                    layout(location = 0) out vec2 fragTexCoord;

                    void main() {
                        gl_Position = vec4(vertexPosition, 1.0);
                        fragTexCoord = vertexTexCoord;
                    }
                )shader";

                const char* fragment_shader_glsl = R"shader(
                    #version 450 core
                    layout(location = 0) in vec2 fragTexCoord;
                    layout(location = 0) out vec4 color;

                    layout(binding = 0) uniform sampler2D texSampler;

                    void main() {
                        color = texture(texSampler, fragTexCoord);
                    }
                )shader";

                MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Compile fragment shader");
                pixel_shader->compile(fragment_shader_glsl);
                MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Compile vertex shader");
                vertex_shader->compile(vertex_shader_glsl);
                MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Shaders compiled");

            } else if (m_render_system->implementation_name() ==
                           "mge::dx11::render_system" ||
                       m_render_system->implementation_name() ==
                           "mge::dx12::render_system") {

                const char* vertex_shader_hlsl = R"shader(
                    struct VS_INPUT
                    {
                        float3 pos : POSITION;
                        float2 tex : TEXCOORD;
                    };

                    struct VS_OUTPUT
                    {
                        float4 pos : SV_POSITION;
                        float2 tex : TEXCOORD;
                    };

                    VS_OUTPUT main(VS_INPUT input)
                    {
                        VS_OUTPUT output;
                        output.pos = float4(input.pos, 1.0);
                        output.tex = input.tex;
                        return output;
                    }
                )shader";

                const char* fragment_shader_hlsl = R"shader(
                    Texture2D texSampler : register(t0);
                    SamplerState texSamplerState : register(s0);

                    struct PS_INPUT
                    {
                        float4 pos : SV_POSITION;
                        float2 tex : TEXCOORD;
                    };

                    float4 main(PS_INPUT input) : SV_TARGET
                    {
                        return texSampler.Sample(texSamplerState, input.tex);
                    }
                )shader";

                MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Compile fragment shader");
                pixel_shader->compile(fragment_shader_hlsl);
                MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Compile vertex shader");
                vertex_shader->compile(vertex_shader_hlsl);
                MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Shaders compiled");

            } else {
                MGE_ERROR_TRACE(TEXTURED_TRIANGLE,
                                "Cannot create shaders for {} render system",
                                m_render_system->implementation_name());
                MGE_THROW(mge::illegal_state) << "Cannot create shaders";
            }
            m_program->set_shader(pixel_shader);
            m_program->set_shader(vertex_shader);
            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Linking program");
            m_program->link();
            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE,
                            "Program linked: {}",
                            m_program->needs_link() ? "needs link"
                                                    : "linked successfully");

            // Vertex data: position (vec3) + texcoord (vec2)
            // UV convention: DX/Vulkan (V=0 at top, V=1 at bottom)
            // Triangle with UV mapping:
            //   top vertex    (0.0, 0.5, 0.0)  -> UV (0.5, 0.0)
            //   bottom-right  (0.45, -0.5, 0.0) -> UV (1.0, 1.0)
            //   bottom-left   (-0.45, -0.5, 0.0) -> UV (0.0, 1.0)
            float vertex_data[] = {
                // position          // texcoord
                0.0f,
                0.5f,
                0.0f,
                0.5f,
                0.0f,
                0.45f,
                -0.5f,
                0.0f,
                1.0f,
                1.0f,
                -0.45f,
                -0.5f,
                0.0f,
                0.0f,
                1.0f,
            };
            int triangle_indices[] = {0, 1, 2};

            mge::vertex_layout layout;
            layout.push_back(mge::vertex_format(mge::data_type::FLOAT, 3),
                             mge::attribute_semantic::POSITION);
            layout.push_back(mge::vertex_format(mge::data_type::FLOAT, 2),
                             mge::attribute_semantic::TEXCOORD);

            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Create vertex buffer");
            m_vertices =
                ctx.create_vertex_buffer(layout,
                                         sizeof(vertex_data),
                                         mge::make_buffer(vertex_data));

            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Create index buffer");
            m_indices =
                ctx.create_index_buffer(mge::data_type::INT32,
                                        sizeof(triangle_indices),
                                        mge::make_buffer(triangle_indices));

            // Load texture from asset
            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Loading texture");
            mge::asset sand_asset("/images/loremipsum.png");
            auto       loaded = sand_asset.load();
            auto       image = std::any_cast<mge::image_ref>(loaded);
            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE,
                            "Image loaded: {}x{}",
                            image->extent().width,
                            image->extent().height);

            m_texture = ctx.create_texture(mge::texture_type::TYPE_2D);
            m_texture->set_data(*image);
            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Texture created");

            MGE_DEBUG_TRACE(TEXTURED_TRIANGLE, "Initializing objects done");
            m_initialized = true;
        }

    private:
        render_system_ref    m_render_system;
        window_ref           m_window;
        std::atomic<bool>    m_initialized;
        program_handle       m_program;
        vertex_buffer_handle m_vertices;
        index_buffer_handle  m_indices;
        texture_ref          m_texture;
    };

    MGE_REGISTER_IMPLEMENTATION(textured_triangle,
                                mge::application,
                                textured_triangle);
} // namespace mge

MGE_MAINFUNCTION
