// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
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
} // namespace mge::opengl