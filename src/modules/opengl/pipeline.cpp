// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "pipeline.hpp"
#include "error.hpp"
#include "common.hpp"
#include "shader.hpp"
#include "mge/core/log.hpp"
#include "render_context.hpp"

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
        clear_vaos();

        if(m_program) {
            await([&]{
                glDeleteProgram(m_program);
                error::clear();
            });
        }
    }

    void
    pipeline::clear_vaos()
    {
        if (m_program) {
            opengl::render_context& ctx = static_cast<opengl::render_context&>(context());
            ctx.clear_vaos_of_program(gl_program(*this));
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
            collect_attributes();
        });
    }

    void
    pipeline::collect_attributes()
    {
        uint32_t active_attributes = 0;
        uint32_t active_uniforms = 0;
        uint32_t active_attributes_maxlength = 0;
        uint32_t active_uniform_maxlength = 0;
        glGetProgramiv(m_program,
                       GL_ACTIVE_ATTRIBUTES,
                       reinterpret_cast<GLint*>(&active_attributes));
        CHECK_OPENGL_ERROR(glGetProgramiv(GL_ACTIVE_ATTRIBUTES));
        // MGE_DEBUG_LOG(OPENGL) << "Active pipeline attributes: " << active_attributes;
        glGetProgramiv(m_program,
                       GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                       reinterpret_cast<GLint*>(&active_attributes_maxlength));
        CHECK_OPENGL_ERROR(glGetProgramiv(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH));
        // MGE_DEBUG_LOG(OPENGL) << "Active pipeline attribute max length: " << active_attributes_maxlength;
        
        for (GLuint i=0; i<active_attributes; ++i) {
            std::string name; 
            name.resize(active_attributes_maxlength+1);
            GLenum type = 0u;
            GLsizei length = 0;
            GLint size = 0;
            glGetActiveAttrib(m_program,
                              i,
                              static_cast<GLsizei>(name.size()),
                              &length,
                              &size,
                              &type,
                              name.data());
            CHECK_OPENGL_ERROR(glGetActiveAttrib);
            name.resize(static_cast<std::string::size_type>(length));
            m_attributes.emplace_back(mge::pipeline::attribute{name, data_type_of_gl_type(type), static_cast<uint32_t>(size)});
            // MGE_DEBUG_LOG(OPENGL) << "Attribute[" << i << "] = {name=\"" << name << "\", type=" << std::hex << type << ", size=" << size << "}";
        }

        glGetProgramiv(m_program,
                       GL_ACTIVE_UNIFORMS,
                       reinterpret_cast<GLint*>(&active_uniforms));
        CHECK_OPENGL_ERROR(glGetProgramiv(GL_ACTIVE_UNIFORMS));
        //MGE_DEBUG_LOG(OPENGL) << "Active pipeline uniforms: " << active_uniforms;
        glGetProgramiv(m_program,
                       GL_ACTIVE_UNIFORM_MAX_LENGTH,
                       reinterpret_cast<GLint*>(&active_uniform_maxlength));
        for(uint32_t i=0; i<active_uniforms; ++i) {
            std::string name;
            name.resize(active_uniform_maxlength+1);
            GLenum type = 0u;
            GLsizei length = 0;
            GLint size = 0;
            glGetActiveUniform(m_program,
                               i,
                               static_cast<GLsizei>(name.size()),
                               &length,
                               &size,
                               &type,
                               name.data());
            CHECK_OPENGL_ERROR(glGetActiveUniform);
            name.resize(static_cast<std::string::size_type>(length));
            m_uniforms.emplace_back(mge::pipeline::uniform{name, data_type_of_gl_type(type), static_cast<uint32_t>(size)});
        }
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
            infolog.resize(static_cast<std::string::size_type>(loglength) - 1);
            glGetProgramInfoLog(m_program,
                            length,
                            &length,
                            reinterpret_cast<GLchar *>(infolog.data()));
            MGE_DEBUG_LOG(OPENGL) << infolog;
        }
    }
}
