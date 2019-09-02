#include "pipeline.hpp"
#include "error.hpp"
#include "common.hpp"
#include "shader.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(OPENGL);

namespace opengl {
    pipeline::pipeline(mge::render_context& context)
        :mge::pipeline(context)
        ,m_program(0)
    {
        await([&]{
            m_program = glCreateProgram();
            CHECK_OPENGL_ERROR(glCreateProgram);
            if(m_program == 0) {
                MGE_THROW(error) << "glCreateProgram failed";
            }
        });
    }

    pipeline::~pipeline()
    {
        if(m_program) {
            await([&]{
                glDeleteProgram(m_program);
                error::clear();
            });
        }
    }

    void
    pipeline::on_set_shader(const mge::shader_ref& shader_)
    {
        assert_same_context(*shader_);
        if(!shader_->defined()) {
            MGE_THROW(mge::illegal_state) << "Cannot apply undefined shader to pipeline";
        }

        const shader *opengl_shader = static_cast<const shader *>(shader_.get());
        await([&]{
            glAttachShader(m_program, opengl_shader->gl_shader());
            CHECK_OPENGL_ERROR(glAttachShader);
        });
    }

    void
    pipeline::on_link()
    {
        await([&]{
            glLinkProgram(m_program);
            CHECK_OPENGL_ERROR(glLinkProgram);
            GLint link_status = GL_FALSE;
            dump_info_log();
            glGetProgramiv(m_program,
                           GL_LINK_STATUS,
                           &link_status);
            if(!link_status) {
                MGE_THROW(error) << "glLinkProgram failed";
            }
        });
    }

    void
    pipeline::dump_info_log()
    {
        GLint loglength = 0;
        glGetProgramiv(m_program,
                        GL_INFO_LOG_LENGTH,
                        &loglength);
        if (loglength == 0) {
            MGE_DEBUG_LOG(OPENGL) << "No program info log";
        } else {
            GLsizei length = loglength;
            std::string infolog;
            infolog.resize(loglength - 1);
            glGetProgramInfoLog(m_program,
                            length,
                            &length,
                            (GLchar *)infolog.data());
            MGE_DEBUG_LOG(OPENGL) << infolog;
        }
    }
}