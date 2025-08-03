// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "program.hpp"
#include "error.hpp"
#include "mge/graphics/data_type.hpp" // Include data_type.hpp
#include "render_context.hpp"
#include "shader.hpp"

namespace mge {
    MGE_USE_TRACE(OPENGL);
}

static mge::uniform_data_type uniform_type_from_gl(GLenum t)
{
    switch (t) {
    case GL_FLOAT:
        return mge::uniform_data_type::FLOAT;
    case GL_FLOAT_VEC2:
        return mge::uniform_data_type::FLOAT_VEC2;
    case GL_FLOAT_VEC3:
        return mge::uniform_data_type::FLOAT_VEC3;
    case GL_FLOAT_VEC4:
        return mge::uniform_data_type::FLOAT_VEC4;
    case GL_INT:
        return mge::uniform_data_type::INT32;
    case GL_INT_VEC2:
        return mge::uniform_data_type::INT_VEC2;
    case GL_INT_VEC3:
        return mge::uniform_data_type::INT_VEC3;
    case GL_INT_VEC4:
        return mge::uniform_data_type::INT_VEC4;
    case GL_UNSIGNED_INT:
        return mge::uniform_data_type::UINT32; // Use UINT32
    case GL_UNSIGNED_INT_VEC2:
        return mge::uniform_data_type::UINT_VEC2;
    case GL_UNSIGNED_INT_VEC3:
        return mge::uniform_data_type::UINT_VEC3;
    case GL_UNSIGNED_INT_VEC4:
        return mge::uniform_data_type::UINT_VEC4;
    case GL_BOOL:
    case GL_BOOL_VEC2:
    case GL_BOOL_VEC3:
    case GL_BOOL_VEC4:
        return mge::uniform_data_type::UNKNOWN; // No boolean types in data_type
    case GL_FLOAT_MAT2:
        return mge::uniform_data_type::FLOAT_MAT2;
    case GL_FLOAT_MAT3:
        return mge::uniform_data_type::FLOAT_MAT3;
    case GL_FLOAT_MAT4:
        return mge::uniform_data_type::FLOAT_MAT4;
    case GL_FLOAT_MAT2x3:
        return mge::uniform_data_type::FLOAT_MAT2x3;
    case GL_FLOAT_MAT2x4:
        return mge::uniform_data_type::FLOAT_MAT2x4;
    case GL_FLOAT_MAT3x2:
        return mge::uniform_data_type::FLOAT_MAT3x2;
    case GL_FLOAT_MAT3x4:
        return mge::uniform_data_type::FLOAT_MAT3x4;
    case GL_FLOAT_MAT4x2:
        return mge::uniform_data_type::FLOAT_MAT4x2;
    case GL_FLOAT_MAT4x3:
        return mge::uniform_data_type::FLOAT_MAT4x3;
    // Sampler types are not represented in mge::uniform_data_type
    case GL_SAMPLER_1D:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_3D:
    case GL_SAMPLER_CUBE:
    case GL_SAMPLER_1D_SHADOW:
    case GL_SAMPLER_2D_SHADOW:
    case GL_SAMPLER_1D_ARRAY:
    case GL_SAMPLER_2D_ARRAY:
    case GL_SAMPLER_1D_ARRAY_SHADOW:
    case GL_SAMPLER_2D_ARRAY_SHADOW:
    case GL_SAMPLER_2D_MULTISAMPLE:
    case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
    case GL_SAMPLER_CUBE_SHADOW:
    case GL_SAMPLER_BUFFER:
    case GL_SAMPLER_2D_RECT:
    case GL_SAMPLER_2D_RECT_SHADOW:
    case GL_INT_SAMPLER_1D:
    case GL_INT_SAMPLER_2D:
    case GL_INT_SAMPLER_3D:
    case GL_INT_SAMPLER_CUBE:
    case GL_INT_SAMPLER_1D_ARRAY:
    case GL_INT_SAMPLER_2D_ARRAY:
    case GL_INT_SAMPLER_2D_MULTISAMPLE:
    case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
    case GL_INT_SAMPLER_BUFFER:
    case GL_INT_SAMPLER_2D_RECT:
    case GL_UNSIGNED_INT_SAMPLER_1D:
    case GL_UNSIGNED_INT_SAMPLER_2D:
    case GL_UNSIGNED_INT_SAMPLER_3D:
    case GL_UNSIGNED_INT_SAMPLER_CUBE:
    case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
    case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
    case GL_UNSIGNED_INT_SAMPLER_BUFFER:
    case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
        return mge::uniform_data_type::UNKNOWN;
    default:
        return mge::uniform_data_type::UNKNOWN;
    }
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
        collect_uniform_buffers();
    }

    void program::dump_info_log()
    {
        if (MGE_DEBUG_TRACE_ENABLED(OPENGL)) {
            GLint length = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
            CHECK_OPENGL_ERROR(glGetProgramiv(GL_INFO_LOG_LENGTH));
            if (length == 0) {
                MGE_DEBUG_TRACE(OPENGL, "No program info log");
                return;
            }
            std::vector<char> infolog(length);
            glGetProgramInfoLog(m_program, length, nullptr, infolog.data());
            CHECK_OPENGL_ERROR(glGetProgramInfoLog);
            MGE_DEBUG_TRACE(OPENGL,
                            "{}",
                            std::string(infolog.data(), infolog.size()));
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
        m_uniforms.clear(); // Assuming m_uniforms is the member variable

        GLint num_uniforms = 0;
        glGetProgramInterfaceiv(m_program,
                                GL_UNIFORM,
                                GL_ACTIVE_RESOURCES,
                                &num_uniforms);
        CHECK_OPENGL_ERROR(glGetProgramInterfaceiv(GL_ACTIVE_RESOURCES));

        if (num_uniforms == 0) {
            MGE_DEBUG_TRACE(OPENGL,
                            "No active uniforms found in program {}",
                            m_program);
            return;
        }

        GLint max_name_length = 0;
        glGetProgramInterfaceiv(m_program,
                                GL_UNIFORM,
                                GL_MAX_NAME_LENGTH,
                                &max_name_length);
        CHECK_OPENGL_ERROR(glGetProgramInterfaceiv(GL_MAX_NAME_LENGTH));

        std::vector<char> name_buffer(max_name_length);

        MGE_DEBUG_TRACE(OPENGL,
                        "Found {} uniforms in program {}",
                        num_uniforms,
                        m_program);

        const GLenum properties[] = {GL_NAME_LENGTH,
                                     GL_TYPE,
                                     GL_ARRAY_SIZE,
                                     GL_LOCATION,
                                     GL_BLOCK_INDEX};
        const int    num_props = sizeof(properties) / sizeof(properties[0]);

        for (GLint i = 0; i < num_uniforms; ++i) {
            GLint values[num_props];
            glGetProgramResourceiv(m_program,
                                   GL_UNIFORM,
                                   i,
                                   num_props,
                                   properties,
                                   num_props,
                                   nullptr,
                                   values);
            CHECK_OPENGL_ERROR(glGetProgramResourceiv);

            // Skip uniforms in blocks (handled separately if needed)
            if (values[4] != -1) {
                continue;
            }

            GLsizei name_length = 0;
            glGetProgramResourceName(m_program,
                                     GL_UNIFORM,
                                     i,
                                     static_cast<GLsizei>(name_buffer.size()),
                                     &name_length,
                                     name_buffer.data());
            CHECK_OPENGL_ERROR(glGetProgramResourceName);

            std::string name(name_buffer.data(), name_length);
            GLenum      gl_type = values[1];
            GLint       array_size = values[2];
            GLint       location = values[3];
            auto uniform_type = static_cast<mge::uniform_data_type>(values[1]);
            uniform_type = uniform_type_from_gl(gl_type);

            if (uniform_type ==
                mge::uniform_data_type::UNKNOWN) { // Check against
                                                   // mge::data_type::UNKNOWN
                MGE_WARNING_TRACE(
                    OPENGL,
                    "Unsupported uniform type {} for uniform '{}'",
                    gl_type,
                    name);
                continue;
            }

            if (name.ends_with("]")) {
                // Remove array suffix from name
                auto pos = name.find_last_of('[');
                if (pos != std::string::npos) {
                    name.erase(pos);
                }
                if (array_size == 1) {
                    MGE_WARNING_TRACE(OPENGL,
                                      "Array size of uniform '{}' is 1, but "
                                      "array notation is used. "
                                      "Consider removing the array notation.",
                                      name);
                }
            }

            if (array_size == 0) {
                array_size = 1; // Default to 1 if no array size is specified
            }

            m_uniforms.push_back({std::move(name),
                                  uniform_type,
                                  static_cast<uint32_t>(location),
                                  static_cast<uint32_t>(array_size)});
            MGE_DEBUG_TRACE(
                OPENGL,
                "Uniform: '{}', type: {}, location: {}, array_size: {}",
                m_uniforms.back().name,
                m_uniforms.back().type,
                m_uniforms.back().location,
                m_uniforms.back().array_size);
        }
    }

    void program::collect_attributes()
    {
        // Clear previous attributes
        m_attributes.clear(); // Assuming m_attributes is the member variable

        GLint active_attribute_max_length = 0;
        glGetProgramiv(m_program,
                       GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                       &active_attribute_max_length);
        CHECK_OPENGL_ERROR(glGetProgramiv(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH));
        // Use std::vector instead of alloca for safer memory management
        std::vector<char> namebuffer(active_attribute_max_length + 1);
        GLint             num_attributes = 0;
        glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &num_attributes);
        CHECK_OPENGL_ERROR(glGetProgramiv(GL_ACTIVE_ATTRIBUTES));
        MGE_DEBUG_TRACE(OPENGL,
                        "Found {} attributes in program {}",
                        num_attributes,
                        m_program);
        for (GLint i = 0; i < num_attributes; ++i) {
            GLint   size = 0;
            GLenum  type = 0;
            GLsizei length = 0;
            glGetActiveAttrib(m_program,
                              i,
                              static_cast<GLsizei>(namebuffer.size()),
                              &length,
                              &size,
                              &type,
                              namebuffer.data());
            CHECK_OPENGL_ERROR(glGetActiveAttrib);
            auto attr_type = attribute_type_from_gl(type);
            if (attr_type == mge::data_type::UNKNOWN) {
                MGE_WARNING_TRACE(
                    OPENGL,
                    "Unsupported attribute type {} for attribute {}",
                    type,
                    namebuffer.data());
                continue; // Skip unsupported types
            }
            // Assuming m_attributes is std::vector<mge::vertex_attribute>
            // and vertex_attribute has members: name, type, size
            m_attributes.push_back(
                {namebuffer.data(), attr_type, static_cast<uint8_t>(size)});
            MGE_DEBUG_TRACE(OPENGL,
                            "Attribute: '{}', type: {}, size: {}",
                            namebuffer.data(),
                            attr_type,
                            size);
        }
    }

    void program::collect_uniform_buffers()
    {
        // Clear previous uniform buffers
        m_uniform_buffers
            .clear(); // Assuming m_uniform_buffers is the member variable

        GLint num_uniform_blocks = 0;
        glGetProgramInterfaceiv(m_program,
                                GL_UNIFORM_BLOCK,
                                GL_ACTIVE_RESOURCES,
                                &num_uniform_blocks);
        CHECK_OPENGL_ERROR(glGetProgramInterfaceiv(GL_ACTIVE_RESOURCES));

        if (num_uniform_blocks == 0) {
            MGE_DEBUG_TRACE(OPENGL,
                            "No active uniform blocks found in program {}",
                            m_program);
            return;
        }

        GLint max_name_length = 0;
        glGetProgramInterfaceiv(m_program,
                                GL_UNIFORM_BLOCK,
                                GL_MAX_NAME_LENGTH,
                                &max_name_length);
        CHECK_OPENGL_ERROR(glGetProgramInterfaceiv(GL_MAX_NAME_LENGTH));

        std::vector<char> name_buffer(max_name_length);

        MGE_DEBUG_TRACE(OPENGL,
                        "Found {} uniform blocks in program {}",
                        num_uniform_blocks,
                        m_program);
        const GLenum properties[] = {GL_NUM_ACTIVE_VARIABLES};
        const int    num_props = sizeof(properties) / sizeof(properties[0]);

        for (GLint i = 0; i < num_uniform_blocks; ++i) {
            GLint values[num_props];
            glGetProgramResourceiv(m_program,
                                   GL_UNIFORM_BLOCK,
                                   i,
                                   num_props,
                                   properties,
                                   num_props,
                                   nullptr,
                                   values);
            CHECK_OPENGL_ERROR(glGetProgramResourceiv);
            GLint   num_uniforms = values[0];
            GLsizei name_length = 0;
            glGetProgramResourceName(m_program,
                                     GL_UNIFORM_BLOCK,
                                     i,
                                     static_cast<GLsizei>(name_buffer.size()),
                                     &name_length,
                                     name_buffer.data());
            CHECK_OPENGL_ERROR(glGetProgramResourceName);

            std::string name(name_buffer.data(), name_length);

            program::uniform_buffer ub;
            ub.name = name;
            m_uniform_buffers.push_back(ub);

            MGE_DEBUG_TRACE(OPENGL,
                            "Uniform block: '{}' has {} uniforms",
                            name,
                            num_uniforms);

            for (GLint j = 0; j < num_uniforms; ++j) {
            }
        }
    }
} // namespace mge::opengl