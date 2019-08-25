// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "shader.hpp"
#include "error.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(OPENGL);

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
    shader::on_compile(const std::string &source_code)
    {
        if(source_code.empty()) {
            MGE_THROW(opengl::error) << "Empty shader source code";
        }
        await([&]{
            if(!m_shader) {
                create_shader();
            }

            const char *src_ptr = source_code.c_str();
            GLint src_length = static_cast<GLint>(source_code.size());
            glShaderSource(m_shader, 1, &src_ptr, &src_length);
            CHECK_OPENGL_ERROR(glShaderSource);

            glCompileShader(m_shader);
            CHECK_OPENGL_ERROR(glCompileShader);
            GLint success = GL_FALSE;
            glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE) {
                throw_compilation_error();
            }
        });
    }

    void
    shader::throw_compilation_error()
    {
        GLint log_length = 0;
        glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<GLchar> log((size_t) log_length + 1);
        glGetShaderInfoLog(m_shader, log_length, &log_length, &log[0]);

        MGE_ERROR_LOG(OPENGL)
            << "Shader compilation failed: " << std::endl
            << &(log[0]) << std::endl;


        MGE_THROW(opengl::error) << "Shader compilation failed:"
              << &(log[0]);
    }

    void
    shader::on_load(const mge::buffer &code)
    {
        // glShaderBinary()
    }

    void
    shader::create_shader()
    {
        m_shader = glCreateShader(gl_shader_type());
        CHECK_OPENGL_ERROR(glCreateShader);
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
