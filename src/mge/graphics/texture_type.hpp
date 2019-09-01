#pragma once
#include "mge/graphics/dllexport.hpp"
#include <iosfwd>

namespace mge {

    /**
     * Texture type, which describes the nature of a texture.
     */
    enum class texture_type
    {
        TEXTURE_1D,
        TEXTURE_2D,
        TEXTURE_3D,
        TEXTURE_CUBE_MAP
    };

    MGE_GRAPHICS_EXPORT std::ostream& operator << (std::ostream& os,
                                                   const texture_type& t);

}
