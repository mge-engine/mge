// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "shader.hpp"
#include "error.hpp"

namespace opengl {

    shader::shader(mge::render_context& context,
                   mge::shader_type type)
        :mge::shader(context, type)
        ,m_shader(0)
    {}

    shader::~shader()
    {
        if(m_shader) {
            await([&]{
                glDeleteShader(m_shader);
                error::clear();
            });
        }
    }

    void
    shader::compile(const std::string &source_code)
    {
        if(source_code.empty()) {
            MGE_THROW(opengl::error) << "Empty shader source code";
        }
        await([&]{
            create_shader();
        });
    }

    void
    shader::load(const mge::buffer &code)
    {
        // glShaderBinary()
    }

    void
    shader::create_shader()
    {
        m_shader = glCreateShader(gl_shader_type());
    }

    GLenum
    shader::gl_shader_type() const
    {
        switch (type()) {
        case mge::shader_type::COMPUTE:
            return GL_COMPUTE_SHADER;
        case mge::shader_type::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        case mge::shader_type::VERTEX:
            return GL_VERTEX_SHADER;
        case mge::shader_type::GEOMETRY:
            return GL_GEOMETRY_SHADER;
        case mge::shader_type::TESSELATION_CONTROL:
            return GL_TESS_CONTROL_SHADER;
        case mge::shader_type::TESSELATION_EVALUATION:
            return GL_TESS_EVALUATION_SHADER;
        default:
            MGE_THROW(::opengl::error)
              << "Invalid shader type: " << type();
        }
    }
}
