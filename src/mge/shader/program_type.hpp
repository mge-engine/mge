#pragma once
#include "mge/shader/dllexport.hpp"
namespace mge {
namespace shader {

    /**
     * Shader program type.
     */
    enum class program_type
    {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        COMPUTE,
        TESSELATION_CONTROL,
        TESSELATION_EVALUATION
    };
}
}
