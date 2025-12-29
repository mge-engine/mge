// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/shader.hpp"
#include "opengl_test.hpp"

class program_test : public mge::opengl::opengltest
{};

TEST_F(program_test, compile_and_link)
{
    const char* vertex_shader_glsl = R"shader(
                    #version 330 core
                    layout(location = 0) in vec3 vertexPosition;
                    void main() {
                      gl_Position.xyz = vertexPosition;
                      gl_Position.w = 1.0;
                    }
                )shader";

    const char* fragment_shader_glsl = R"shader(
                    #version 330 core
                    #extension GL_ARB_explicit_uniform_location : enable
                    out vec3 color;

                    struct a_struct
                    {
                        vec3 first;
                        vec4 second;
                        mat4x4 third;
                    };

                    uniform vec3 one_uniform;
                    layout(location = 7) uniform a_struct two_uniform[5];
                    uniform float third_uniform[3];

                    uniform ExtraBlock  {
                        float extra1;
                        float extra2;
                    } extra_uniforms;

                    void main() {
                        color = one_uniform + two_uniform[3].first
                        + vec3(two_uniform[2].second.x, 1, 1);
                        color.x += third_uniform[0] + extra_uniforms.extra1;
                        color.y += third_uniform[1] + extra_uniforms.extra2;
                        color.z += third_uniform[2];
                    }
                )shader";

    auto pixel_shader =
        m_window->render_context().create_shader(mge::shader_type::FRAGMENT);
    pixel_shader->compile(fragment_shader_glsl);
    auto vertex_shader =
        m_window->render_context().create_shader(mge::shader_type::VERTEX);
    vertex_shader->compile(vertex_shader_glsl);
    auto program = m_window->render_context().create_program();
    program->set_shader(pixel_shader);
    program->set_shader(vertex_shader);
    program->link();
    m_window->render_context().frame();

    ASSERT_EQ(1u, program->attributes().size());
    EXPECT_STREQ("vertexPosition", program->attributes()[0].name.c_str());
    EXPECT_EQ(mge::data_type::FLOAT_VEC3, program->attributes()[0].type);
    EXPECT_EQ(1u, program->attributes()[0].size);
}
