// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_source.hpp"
#include "mge/core/buffer.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/frame_buffer_info.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/shader.hpp"

namespace mge {
    MGE_DEFINE_TRACE(HEADLESS);
}

namespace mge {


    class headless : public application
    {
    public:
        headless() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(HEADLESS, "Setup headless");

            mge::properties p;
            p.set("directory", "./assets");
            mge::asset::mount("/", "file", p);
            p.set("directory", "./temp");
            mge::asset::mount("/temp",
                              "file",
                              mge::asset_source::access_mode::READ_WRITE,
                              p);

            m_render_system = render_system::create();
            m_context =
                m_render_system->create_headless_render_context(m_extent);

            mge::frame_buffer_info fb_info;
            fb_info.color_attachments.push_back(
                {mge::image_format(mge::image_format::data_format::RGBA,
                                   mge::data_type::UINT8),
                 m_extent});
            m_frame_buffer = m_context->create_frame_buffer(fb_info);

            initialize_shaders();
            initialize_geometry();
        }

        void run() override
        {
            MGE_DEBUG_TRACE(HEADLESS, "Rendering triangle");

            auto& pass = m_context->pass(0);
            pass.set_frame_buffer(m_frame_buffer);
            pass.set_viewport(m_context->default_viewport());
            pass.set_scissor(m_context->default_scissor());
            pass.clear_color(mge::rgba_color(0.0f, 0.0f, 0.2f, 1.0f));
            pass.disable_clear_depth();

            auto& cmd = m_context->command_buffer(true);
            cmd.draw(pass, m_program, m_vertices, m_indices);

            m_context->frame();

            MGE_DEBUG_TRACE(HEADLESS, "Saving screenshot");
            auto img = m_context->screenshot();
            mge::asset output("/temp/headless.png");
            output.store(mge::asset_type("image", "png"), img);
            MGE_INFO_TRACE(HEADLESS, "Saved temp/headless.png");
        }

        void teardown() override
        {
            m_program       = {};
            m_vertices      = {};
            m_indices       = {};
            m_frame_buffer  = {};
            m_context.reset();
            m_render_system.reset();
        }

    private:
        void initialize_shaders()
        {
            MGE_DEBUG_TRACE(HEADLESS,
                            "Initializing shaders for {}",
                            m_render_system->implementation_name());

            auto vertex_shader =
                m_context->create_shader(shader_type::VERTEX);
            auto pixel_shader =
                m_context->create_shader(shader_type::FRAGMENT);
            m_program = m_context->create_program();

            if (m_render_system->implementation_name() ==
                    "mge::opengl::render_system" ||
                m_render_system->implementation_name() ==
                    "mge::vulkan::render_system") {
                const char* vertex_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) in vec3 vertexPosition;
                    void main() {
                        gl_Position.xyz = vertexPosition;
                        gl_Position.w = 1.0;
                    }
                )shader";
                const char* fragment_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) out vec4 color;
                    void main() {
                        color = vec4(1.0, 0.0, 0.0, 1.0);
                    }
                )shader";
                vertex_shader->compile(vertex_glsl);
                pixel_shader->compile(fragment_glsl);
            } else if (m_render_system->implementation_name() ==
                           "mge::dx11::render_system" ||
                       m_render_system->implementation_name() ==
                           "mge::dx12::render_system") {
                const char* vertex_hlsl = R"shader(
                    float4 main(float3 pos : POSITION) : SV_POSITION
                    {
                        return float4(pos, 1.0);
                    }
                )shader";
                const char* fragment_hlsl = R"shader(
                    float4 main() : SV_TARGET
                    {
                        return float4(1.0f, 0.0f, 0.0f, 1.0f);
                    }
                )shader";
                vertex_shader->compile(vertex_hlsl);
                pixel_shader->compile(fragment_hlsl);
            } else {
                MGE_THROW(mge::illegal_state)
                    << "No shader source for render system: "
                    << m_render_system->implementation_name();
            }

            m_program->set_shader(vertex_shader);
            m_program->set_shader(pixel_shader);
            m_program->link();
            MGE_DEBUG_TRACE(HEADLESS, "Shaders compiled and linked");
        }

        void initialize_geometry()
        {
            float triangle_coords[] = {
                0.0f,   0.5f,  0.0f,
                0.45f, -0.5f,  0.0f,
               -0.45f, -0.5f,  0.0f,
            };
            int triangle_indices[] = {0, 1, 2};

            mge::vertex_layout layout;
            layout.push_back(mge::vertex_format(mge::data_type::FLOAT, 3));

            m_vertices = m_context->create_vertex_buffer(
                layout,
                sizeof(triangle_coords),
                mge::make_buffer(triangle_coords));
            m_indices = m_context->create_index_buffer(
                mge::data_type::INT32,
                sizeof(triangle_indices),
                mge::make_buffer(triangle_indices));
            MGE_DEBUG_TRACE(HEADLESS, "Geometry created");
        }

        mge::extent          m_extent{800, 600};
        render_system_ref    m_render_system;
        render_context_ref   m_context;
        frame_buffer_handle  m_frame_buffer;
        program_handle       m_program;
        vertex_buffer_handle m_vertices;
        index_buffer_handle  m_indices;
    };

    MGE_REGISTER_IMPLEMENTATION(headless, mge::application, headless);

} // namespace mge

MGE_MAINFUNCTION
