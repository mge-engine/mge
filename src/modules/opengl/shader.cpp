// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "shader.hpp"
#include "error.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"

namespace mge {
    MGE_USE_TRACE(OPENGL);
}

namespace mge::opengl {

    shader::shader(render_context& context, shader_type type)
        : mge::shader(context, type)
        , m_shader(0)
    {}

    shader::~shader()
    {
        if (m_shader) {
            glDeleteShader(m_shader);
            TRACE_OPENGL_ERROR(glDeleteShader);
            m_shader = 0;
        }
    }

    void shader::on_compile(std::string_view source)
    {
        if (source.empty()) {
            MGE_THROW(opengl::error) << "Empty shader source code";
        }

        if (!m_shader) {
            create_shader();
        }

        const char* src_ptr = source.data();
        GLint       src_length = static_cast<GLint>(source.size());
        glShaderSource(m_shader, 1, &src_ptr, &src_length);
        CHECK_OPENGL_ERROR(glShaderSource);

        glCompileShader(m_shader);
        CHECK_OPENGL_ERROR(glCompileShader);
        GLint success = GL_FALSE;
        glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            throw_compilation_error();
        }
    }

    void shader::throw_compilation_error()
    {
        GLint log_length = 0;
        glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<GLchar> log((size_t)log_length + 1);
        glGetShaderInfoLog(m_shader, log_length, &log_length, &log[0]);
        MGE_ERROR_TRACE(OPENGL, "Shader compilation failed: {}", &(log[0]));
        MGE_THROW(opengl::error) << "Shader compilation failed: " << &(log[0]);
    }

    void shader::create_shader()
    {
        m_shader = glCreateShader(gl_shader_type());
        CHECK_OPENGL_ERROR(glCreateShader);
    }

    void shader::on_set_code(const mge::buffer& code)
    {
        if (glShaderBinary) {
            glShaderBinary(1,
                           &m_shader,
                           GL_SHADER_BINARY_FORMAT_SPIR_V,
                           code.data(),
                           static_cast<GLsizei>(code.size()));
            CHECK_OPENGL_ERROR(glShaderBinary);
            std::string entry_point = get_property("entry_point", "main");
            glSpecializeShader(m_shader,
                               entry_point.c_str(),
                               0,
                               nullptr,
                               nullptr);
            CHECK_OPENGL_ERROR(glSpecializeShader);
        } else {
            MGE_THROW(opengl::error) << "Compiled SPIR-V shaders not supported";
        }
    }

    GLenum shader::gl_shader_type() const
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
            MGE_THROW(error) << "Invalid shader type: " << type();
        }
    }

} // namespace mge::opengl