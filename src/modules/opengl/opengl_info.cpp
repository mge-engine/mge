// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "opengl_info.hpp"
#include "boost/boost_algorithm_string.hpp"
#include "error.hpp"
#include "mge/core/semantic_version.hpp"
#include "mge/core/trace.hpp"
#include "opengl.hpp"

#include <iostream>
#include <sstream>

namespace mge {
    MGE_USE_TRACE(OPENGL);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, opengl, debug, "Enable OpenGL debug", false);
} // namespace mge

namespace mge::opengl {

    opengl_info::opengl_info()
        : major_version(0)
        , minor_version(0)
    {

        glGetIntegerv(GL_MAJOR_VERSION, &major_version);
        glGetIntegerv(GL_MINOR_VERSION, &minor_version);

        MGE_TRACE_OBJECT(OPENGL, INFO)
            << "OpenGL version " << major_version << "." << minor_version;

        glsl_version_str =
            (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

        auto vit = glsl_version_str.begin();
        while (vit != glsl_version_str.end() &&
               (*vit == '.' || std::isdigit(*vit))) {
            ++vit;
        }
        std::string plain_glsl_version(glsl_version_str.begin(), vit);

        shader_language =
            mge::shader_language("glsl",
                                 mge::semantic_version(plain_glsl_version));

        MGE_TRACE_OBJECT(OPENGL, INFO) << "GLSL version " << glsl_version_str;

        int num_shader_formats = 0;
        glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &num_shader_formats);
        MGE_TRACE_OBJECT(OPENGL, INFO)
            << "System supports " << num_shader_formats
            << " binary shader formats";
        if (num_shader_formats) {
            std::vector<int> formats;
            formats.resize(num_shader_formats);
            glGetIntegerv(GL_SHADER_BINARY_FORMATS, formats.data());
            for (const auto& format : formats) {
                switch (format) {
                case GL_SHADER_BINARY_FORMAT_SPIR_V:
                    shader_formats.emplace_back(shader_format("SPIRV"));
                    MGE_TRACE_OBJECT(OPENGL, INFO)
                        << "Binary shader format SPIRV";
                    break;
                default:
                    MGE_TRACE_OBJECT(OPENGL, WARNING)
                        << "Binary shader format 0x" << std::hex << format
                        << std::dec << " is not supported";
                }
            }
        }

        std::string extension_string =
            ((const char*)glGetString(GL_EXTENSIONS));

        auto it =
            boost::make_split_iterator(extension_string,
                                       boost::token_finder(boost::is_space()));

        MGE_TRACE_OBJECT(OPENGL, INFO) << "OpenGL extensions:";
        while (!it.eof()) {
            if (!it->empty()) {
                MGE_TRACE_OBJECT(OPENGL, INFO) << *it;
                extensions.insert(std::string(it->begin(), it->end()));
            }
            ++it;
        }

        if (/* MGE_PARAMETER(opengl, debug).get() && */
            extensions.find("GL_ARB_debug_output") != extensions.end()) {
            install_debug_callback();
        }
    }

    static const char* source_name(GLenum source)
    {
        switch (source) {
        case GL_DEBUG_SOURCE_API_ARB:
            return "GL";
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
            return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
            return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_APPLICATION_ARB:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
            return "3RDPARTY";
        case GL_DEBUG_SOURCE_OTHER_ARB:
            return "OTHER";
        default:
            return "UNKNOWN";
        }
    }

    static const char* type_name(GLenum type)
    {
        switch (type) {
        case GL_DEBUG_TYPE_ERROR_ARB:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
            return "DEPRECATED";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
            return "UNDEFINED";
        case GL_DEBUG_TYPE_PORTABILITY_ARB:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_OTHER_ARB:
            return "OTHER";
        default:
            return "UNKNOWN";
        }
    }

    static const char* severity_name(GLenum severity)
    {
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH_ARB:
            return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW_ARB:
            return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
        default:
            return "UNKNOWN";
        }
    }

    static void debug_callback(GLenum        source,
                               GLenum        type,
                               GLuint        id,
                               GLenum        severity,
                               GLsizei       length,
                               const GLchar* message,
                               const void*   user_param)
    {
        const char* srcname = source_name(source);
        const char* typename_str = type_name(type);
        const char* severity_str = severity_name(severity);

        // Format the message with all available information
        std::stringstream ss;
        ss << "OpenGL Debug [" << severity_str << "] "
           << "(" << srcname << ":" << typename_str << ") "
           << "ID: " << id << " - " << message;

        // Log based on severity
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH_ARB:
            MGE_ERROR_TRACE(OPENGL,
                            "OpenGL Debug [{}] ({}) ID: {} - {}",
                            severity_str,
                            srcname,
                            id,
                            message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:
            MGE_WARNING_TRACE(OPENGL,
                              "OpenGL Debug [{}] ({}) ID: {} - {}",
                              severity_str,
                              srcname,
                              id,
                              message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            MGE_DEBUG_TRACE_STREAM(OPENGL) << ss.str();
            break;
        case GL_DEBUG_SEVERITY_LOW_ARB:
        default:
            MGE_INFO_TRACE(OPENGL,
                           "OpenGL Debug [{}] ({}) ID: {} - {}",
                           severity_str,
                           srcname,
                           id,
                           message);
            break;
        }
    }

    void opengl_info::install_debug_callback()
    {
        MGE_TRACE_OBJECT(OPENGL, INFO) << "Install debug callback";
        glEnable(GL_DEBUG_OUTPUT);
        TRACE_OPENGL_ERROR(glEnable(GL_DEBUG_OUTPUT));
        glDebugMessageCallback(&debug_callback, this);
        TRACE_OPENGL_ERROR(glDebugMessageCallback);
    }

    opengl_info::~opengl_info() {}

} // namespace mge::opengl
