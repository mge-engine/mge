#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(OPENGL);

namespace opengl {
    MGE_DEFINE_EXCEPTION(error);

    static const char *gl_error_text(GLenum error_code)
    {
        switch(error_code) {
        case GL_NO_ERROR:
            return "No error";
        case GL_INVALID_ENUM:
            return "Invalid enum value";
        case GL_INVALID_VALUE:
            return "Invalid value";
        case GL_INVALID_OPERATION:
            return "Invalid operation";
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "Invalid framebuffer operation";
        case GL_STACK_OVERFLOW:
            return "Stack overflow";
        case GL_STACK_UNDERFLOW:
            return "Stack underflow";
        default:
            return "Unknown error";
        }

    }

    void
    error::clear()
    {
        auto err = glGetError();
        if(err != GL_NO_ERROR) {
            MGE_WARNING_LOG(OPENGL) << "Ignoring OpenGL error: ("
                                    << (unsigned int)err << ") "  << gl_error_text(err);
        }
    }

    void
    error::check_error(const char *file,
                       int line,
                       const char *signature,
                       const char *glFunction)
    {
        auto err = glGetError();
        if(err != GL_NO_ERROR) {
            throw error().set_info(mge::exception::source_file(file))
                    .set_info(mge::exception::source_line(line))
                    .set_info(mge::exception::function(signature))
                    .set_info(mge::exception::stack(mge::stacktrace()))
                    .set_info(mge::exception::type_name(mge::type_name<error>()))
                    .set_info(::mge::exception::called_function(glFunction))
             << "OpenGL error: (" << (unsigned int)err << ") " << gl_error_text(err);
        }
    }
}
