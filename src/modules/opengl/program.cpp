// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "program.hpp"
#include "error.hpp"
#include "mge/graphics/uniform_data_type.hpp"
#include "render_context.hpp"
#include "shader.hpp"

namespace mge {
    MGE_USE_TRACE(OPENGL);
}

static mge::uniform_data_type uniform_type_from_gl(GLenum t)
{
    return mge::uniform_data_type::UNKNOWN;
}

static mge::data_type attribute_type_from_gl(GLenum t)
{
    switch (t) {
    case GL_FLOAT:
        return mge::data_type::FLOAT;
    case GL_FLOAT_VEC2:
        return mge::data_type::FLOAT_VEC2;
    case GL_FLOAT_VEC3:
        return mge::data_type::FLOAT_VEC3;
    case GL_FLOAT_VEC4:
        return mge::data_type::FLOAT_VEC4;
    case GL_FLOAT_MAT2:
        return mge::data_type::FLOAT_MAT2;
    case GL_FLOAT_MAT3:
        return mge::data_type::FLOAT_MAT3;
    case GL_FLOAT_MAT4:
        return mge::data_type::FLOAT_MAT4;
    case GL_FLOAT_MAT2x3:
        return mge::data_type::FLOAT_MAT2x3;
    case GL_FLOAT_MAT2x4:
        return mge::data_type::FLOAT_MAT2x4;
    case GL_FLOAT_MAT3x2:
        return mge::data_type::FLOAT_MAT3x2;
    case GL_FLOAT_MAT3x4:
        return mge::data_type::FLOAT_MAT3x4;
    case GL_FLOAT_MAT4x2:
        return mge::data_type::FLOAT_MAT4x2;
    case GL_FLOAT_MAT4x3:
        return mge::data_type::FLOAT_MAT4x3;
    case GL_INT:
        return mge::data_type::INT32;
    case GL_INT_VEC2:
        return mge::data_type::INT_VEC2;
    case GL_INT_VEC3:
        return mge::data_type::INT_VEC3;
    case GL_INT_VEC4:
        return mge::data_type::INT_VEC4;
    case GL_UNSIGNED_INT:
        return mge::data_type::UINT32;
    case GL_UNSIGNED_INT_VEC2:
        return mge::data_type::UINT_VEC2;
    case GL_UNSIGNED_INT_VEC3:
        return mge::data_type::UINT_VEC3;
    case GL_UNSIGNED_INT_VEC4:
        return mge::data_type::UINT_VEC4;
    case GL_DOUBLE:
        return mge::data_type::DOUBLE;
    case GL_DOUBLE_VEC2:
        return mge::data_type::DOUBLE_VEC2;
    case GL_DOUBLE_VEC3:
        return mge::data_type::DOUBLE_VEC3;
    case GL_DOUBLE_VEC4:
        return mge::data_type::DOUBLE_VEC4;
    case GL_DOUBLE_MAT2:
        return mge::data_type::DOUBLE_MAT2;
    case GL_DOUBLE_MAT3:
        return mge::data_type::DOUBLE_MAT3;
    case GL_DOUBLE_MAT4:
        return mge::data_type::DOUBLE_MAT4;
    case GL_DOUBLE_MAT2x3:
        return mge::data_type::DOUBLE_MAT2x3;
    case GL_DOUBLE_MAT2x4:
        return mge::data_type::DOUBLE_MAT2x4;
    case GL_DOUBLE_MAT3x2:
        return mge::data_type::DOUBLE_MAT3x2;
    case GL_DOUBLE_MAT3x4:
        return mge::data_type::DOUBLE_MAT3x4;
    case GL_DOUBLE_MAT4x2:
        return mge::data_type::DOUBLE_MAT4x2;
    case GL_DOUBLE_MAT4x3:
        return mge::data_type::DOUBLE_MAT4x3;
    default:
        break;
    }
    return mge::data_type::UNKNOWN;
}

namespace mge::opengl {
    program::program(render_context& context)
        : mge::program(context)
        , m_program(0)
    {
        m_program = glCreateProgram();
        CHECK_OPENGL_ERROR(glCreateProgram);
    }

    program::~program()
    {
        if (m_program) {
            glDeleteProgram(m_program);
            TRACE_OPENGL_ERROR(glDeleteProgram);
            m_program = 0;
        }
    }

#if 0
        bool                m_needs_link;
        attribute_list      m_attributes;
        uniform_list        m_uniforms;
        uniform_buffer_list m_uniform_buffers;
#endif

    void program::on_link()
    {
        glLinkProgram(m_program);
        CHECK_OPENGL_ERROR(glLinkProgram);
        GLint link_status = GL_FALSE;
        dump_info_log();
        glGetProgramiv(m_program, GL_LINK_STATUS, &link_status);
        if (!link_status) {
            MGE_THROW(error) << "glLinkProgram failed";
        }
        collect_attributes();
        collect_uniforms();
    }

    void program::dump_info_log()
    {
        if (MGE_DEBUG_TRACE_ENABLED(OPENGL)) {
            GLint loglength = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &loglength);
            if (loglength == 0) {
                MGE_DEBUG_TRACE(OPENGL) << "No program info log";
            } else {
                GLsizei     length = loglength;
                std::string infolog;
                infolog.resize(static_cast<std::string::size_type>(loglength) -
                               1);
                glGetProgramInfoLog(m_program,
                                    length,
                                    &length,
                                    reinterpret_cast<GLchar*>(infolog.data()));
                MGE_DEBUG_TRACE(OPENGL) << infolog;
            }
        }
    }

    void program::on_set_shader(const shader_ref& shader)
    {
        assert_same_context(shader);
        if (!shader->initialized()) {
            MGE_THROW(mge::illegal_state)
                << "Cannot apply uninitialized shader to program";
        }
        const mge::opengl::shader* opengl_shader =
            static_cast<const mge::opengl::shader*>(shader.get());
        glAttachShader(m_program, opengl_shader->gl_shader());
        CHECK_OPENGL_ERROR(glAttachShader);
    }

    void program::collect_uniforms()
    {
        GLint active_uniform_max_length = 0;
        glGetProgramiv(m_program,
                       GL_ACTIVE_UNIFORM_MAX_LENGTH,
                       &active_uniform_max_length);
        CHECK_OPENGL_ERROR(glGetProgramiv(GL_ACTIVE_UNIFORM_MAX_LENGTH));
        char* namebuffer =
            static_cast<char*>(alloca(active_uniform_max_length + 1));
        GLint num_uniforms = 0;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &num_uniforms);
        CHECK_OPENGL_ERROR(glGetProgramiv(GL_ACTIVE_UNIFORMS));
        MGE_DEBUG_TRACE(OPENGL)
            << "Found " << num_uniforms << " uniforms in program " << m_program;
        for (GLint i = 0; i < num_uniforms; ++i) {
            GLint   size = 0;
            GLenum  type = 0;
            GLsizei length = 0;
            glGetActiveUniform(m_program,
                               i,
                               active_uniform_max_length + 1,
                               &length,
                               &size,
                               &type,
                               namebuffer);
            CHECK_OPENGL_ERROR(glGetActiveUniform);
            MGE_DEBUG_TRACE(OPENGL) << "Uniform: " << namebuffer
                                    << ", type: " << type << ", size: " << size;
            uniform_data_type u_type = uniform_type_from_gl(type);
            if (u_type == uniform_data_type::UNKNOWN) {
                MGE_WARNING_TRACE(OPENGL) << "Unsupported uniform type " << type
                                          << " for uniform " << namebuffer;
            } else {
            }
        }
    }

    void program::collect_attributes()
    {
        GLint active_attribute_max_length = 0;
        glGetProgramiv(m_program,
                       GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                       &active_attribute_max_length);
        CHECK_OPENGL_ERROR(glGetProgramiv(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH));
        char* namebuffer =
            static_cast<char*>(alloca(active_attribute_max_length + 1));
        GLint num_attributes = 0;
        glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &num_attributes);
        CHECK_OPENGL_ERROR(glGetProgramiv(GL_ACTIVE_ATTRIBUTES));
        MGE_DEBUG_TRACE(OPENGL) << "Found " << num_attributes
                                << " attributes in program " << m_program;
        for (GLint i = 0; i < num_attributes; ++i) {
            GLint   size = 0;
            GLenum  type = 0;
            GLsizei length = 0;
            glGetActiveAttrib(m_program,
                              i,
                              active_attribute_max_length + 1,
                              &length,
                              &size,
                              &type,
                              namebuffer);
            CHECK_OPENGL_ERROR(glGetActiveAttrib);
            auto attr_type = attribute_type_from_gl(type);
            if (attr_type == mge::data_type::UNKNOWN) {
                MGE_WARNING_TRACE(OPENGL)
                    << "Unsupported attribute type " << type
                    << " for attribute " << namebuffer;
            }
            m_attributes.push_back(
                {namebuffer, attr_type, static_cast<uint8_t>(size)});
            MGE_DEBUG_TRACE(OPENGL)
                << "Attribute: " << namebuffer << ", type: " << attr_type
                << ", size: " << size;
        }
    }
} // namespace mge::opengl