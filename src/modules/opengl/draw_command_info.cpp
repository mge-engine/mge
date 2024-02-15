#include "draw_command_info.hpp"
#include "error.hpp"
#include "mge/core/trace.hpp"
#include "opengl.hpp"

namespace mge {

    MGE_USE_TRACE(OPENGL);
}

namespace mge::opengl {
    draw_command_info::~draw_command_info()
    {
        try {
            if (vao != 0) {
                glDeleteVertexArrays(1, &vao);
                CHECK_OPENGL_ERROR(glDeleteVertexArrays);
            }
        } catch (const mge::exception& ex) {
            MGE_ERROR_TRACE(OPENGL) << ex;
        } catch (const std::exception& ex) {
            MGE_ERROR_TRACE(OPENGL) << ex.what();
        } catch (...) {
            MGE_ERROR_TRACE(OPENGL) << "Unknown error";
        }
    }
} // namespace mge::opengl