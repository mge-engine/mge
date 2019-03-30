#pragma once
#include "mge/graphics/dllexport.hpp"
#include <iosfwd>
namespace mge {


    enum class MGE_GRAPHICS_EXPORT shader_type
    {
        VERTEX,                 //!< vertex
        FRAGMENT,               //!< fragment (pixel)
        COMPUTE,                //!< compute
        TESSELATION_CONTROL,    //!< tesselation control (hull)
        TESSELATION_EVALUATION, //!< tesselation evaluation (domain)
        GEOMETRY,               //!< geometry
        MAX_SHADER_TYPE = GEOMETRY
    };

    MGE_GRAPHICS_EXPORT std::ostream& operator <<(std::ostream& os,
                                                  const shader_type& t);
}
