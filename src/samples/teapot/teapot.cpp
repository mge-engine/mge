// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/asset/asset.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/command_buffer.hpp"
#include "mge/graphics/mesh.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/shader_language.hpp"
#include "mge/graphics/topology.hpp"
#include "mge/graphics/uniform.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "mge/graphics/window.hpp"
#include "mge/math/glm.hpp"
#include "mge/math/mat.hpp"
namespace mge {
    MGE_DEFINE_TRACE(TEAPOT);
}
namespace mge {
    class teapot : public application
    {
    public:
        teapot() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(TEAPOT, "Setup teapot");

            mge::properties p;
            p.set("directory", "./assets");
            mge::asset::mount("/", "file", p);

            m_render_system = render_system::create();
            m_window = m_render_system->create_window();
            m_window->set_close_listener([&] { set_quit(); });
            m_window->add_key_action_handler(
                [&](mge::key k, mge::key_action a, mge::modifier m) -> bool {
                    if (a == mge::key_action::PRESS && k == mge::key::ESCAPE) {
                        set_quit();
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

        void draw(uint64_t cycle, double delta)
        {
            auto& ctx = m_window->render_context();

            if (m_initialized) {
                if (!m_uniform_block && !m_program->needs_link()) {
                    setup_mvp();
                }
            }

            if (m_initialized && m_uniform_block) {
                auto& pass = ctx.pass(0);
                pass.default_viewport();
                pass.default_scissor();
                pass.clear_color(rgba_color(0.0f, 0.0f, 1.0f, 1.0f));
                pass.clear_depth(1.0f);
                auto& command_buffer = ctx.command_buffer(true);
                command_buffer.bind_uniform_block(m_uniform_block.get());
                command_buffer.draw(m_program, m_vertices, m_indices);
                pass.submit(command_buffer);
            } else {
                auto& pass = ctx.pass(0);
                pass.default_viewport();
                pass.clear_color(rgba_color(0.0f, 0.0f, 0.0f, 1.0f));
                pass.touch();
            }
            ctx.frame();
        }

        void initialize()
        {
            MGE_DEBUG_TRACE(TEAPOT, "Initializing objects");

            mge::asset_ref mesh_asset =
                std::make_shared<mge::asset>("/models/teapot.obj");
            mge::mesh_ref mesh;
            try {
                mesh = std::any_cast<mge::mesh_ref>(mesh_asset->load());
            } catch (const std::bad_any_cast& e) {
                MGE_ERROR_TRACE(TEAPOT,
                                "Cannot load teapot mesh: {}",
                                e.what());
                MGE_THROW(mge::illegal_state) << "Cannot load teapot mesh";
            }

            m_program = m_window->render_context().create_program();
            MGE_DEBUG_TRACE(TEAPOT,
                            "render system is {}",
                            m_render_system->implementation_name());

            if (m_render_system->implementation_name() ==
                "mge::opengl::render_system") {
                const char* vertex_shader_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) in vec3 vertexPosition;

                    layout(std140) uniform MVPBlock {
                        mat4 mvp;
                    };

                    void main() {
                      gl_Position = mvp * vec4(vertexPosition, 1.0);
                    }
                )shader";

                const char* fragment_shader_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) out vec3 color;
                    void main() {
                        color = vec3(1,1,1);
                    }
                )shader";

                auto pixel_shader = m_window->render_context().create_shader(
                    shader_type::FRAGMENT);
                auto vertex_shader = m_window->render_context().create_shader(
                    shader_type::VERTEX);
                MGE_DEBUG_TRACE(TEAPOT, "Compile fragment shader");
                pixel_shader->compile(fragment_shader_glsl);
                MGE_DEBUG_TRACE(TEAPOT, "Compile vertex shader");
                vertex_shader->compile(vertex_shader_glsl);
                m_program->set_shader(pixel_shader);
                m_program->set_shader(vertex_shader);
                MGE_DEBUG_TRACE(TEAPOT, "Linking program");
                m_program->link();
                MGE_DEBUG_TRACE(TEAPOT, "Program linked");
            } else if (m_render_system->implementation_name() ==
                           "mge::vulkan::render_system" ||
                       m_render_system->implementation_name() ==
                           "mge::dx11::render_system" ||
                       m_render_system->implementation_name() ==
                           "mge::dx12::render_system") {

                static const char* slang_shader_source = R"slang(
                    cbuffer MVPBlock : register(b0)
                    {
                        float4x4 mvp;
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
                        VertexOutput output;
                        output.position = mul(mvp, float4(input.position, 1.0));
                        return output;
                    }

                    [shader("fragment")]
                    float4 fragmentMain() : SV_TARGET
                    {
                        return float4(1.0f, 1.0f, 1.0f, 1.0f);
                    }
                )slang";

                MGE_DEBUG_TRACE(TEAPOT, "Compile and link SLANG shaders");
                m_program->compile_and_link(mge::shader_language::SLANG,
                                            slang_shader_source);
                MGE_DEBUG_TRACE(TEAPOT, "SLANG shaders compiled");
            } else {
                MGE_ERROR_TRACE(TEAPOT,
                                "Cannot create shaders for {} render system",
                                m_render_system->implementation_name());
                MGE_THROW(mge::illegal_state) << "Cannot create shaders";
            }

            MGE_DEBUG_TRACE(TEAPOT, "Create vertex buffer");
            m_vertices = m_window->render_context().create_vertex_buffer(
                mesh->layout(),
                mesh->vertex_data_size(),
                mesh->vertices());
            MGE_DEBUG_TRACE(TEAPOT, "Create index buffer");
            m_indices = m_window->render_context().create_index_buffer(
                mge::data_type::UINT32,
                mesh->index_data_size(),
                mesh->indices());
            MGE_DEBUG_TRACE(TEAPOT, "Initializing objects done");
            m_initialized = true;
        }

        void setup_mvp()
        {
            const auto& uniform_buffers = m_program->uniform_buffers();
            if (uniform_buffers.empty()) {
                MGE_ERROR_TRACE(TEAPOT, "No uniform buffers found in program");
                return;
            }
            m_uniform_block =
                std::make_unique<mge::uniform_block>(uniform_buffers[0]);

            auto       projection = glm::perspective(glm::radians(45.0f),
                                                     16.0f / 9.0f,
                                                     0.1f,
                                                     100.0f);
            auto       view = glm::lookAt(glm::vec3(0.0f, 2.0f, 8.0f),
                                          glm::vec3(0.0f, 1.5f, 0.0f),
                                          glm::vec3(0.0f, 1.0f, 0.0f));
            auto       model = glm::mat4(1.0f);
            mge::fmat4 mvp = projection * view * model;
            m_uniform_block->set("mvp", mvp);
        }

    private:
        render_system_ref                   m_render_system;
        window_ref                          m_window;
        std::atomic<bool>                   m_initialized;
        program_handle                      m_program;
        vertex_buffer_handle                m_vertices;
        index_buffer_handle                 m_indices;
        std::unique_ptr<mge::uniform_block> m_uniform_block;
    };

    MGE_REGISTER_IMPLEMENTATION(teapot, mge::application, teapot);
} // namespace mge

MGE_MAINFUNCTION
