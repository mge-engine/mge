#pragma once
#include "mge/graphics/dllexport.hpp"
namespace mge {

    /**
     * @brief Draw mode, defines how vertices shall be interpreted.
     */
    enum class draw_mode
    {
        TRIANGLES,
        TRIANGLE_STRIP,
        LINES,
        LINE_STRIP
    };
}
