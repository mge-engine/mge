#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(OPENGL);

namespace opengl {
    MGE_DEFINE_EXCEPTION(error);

    void
    error::clear()
    {
        auto err = glGetError();
        if(err != GL_NO_ERROR) {
            MGE_WARNING_LOG(OPENGL) << "OpenGL error " << err << " ignored";
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
             << "OpenGL error: " << err;
        }
    }
}
