
// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/texture_type.hpp"

namespace mge {

    /**
     * @brief A texture.
     *
     * A texture is a set of data mapped onto a geometry or used in a shader
     * program. Textures can be used for color information, normal information,
     * bump mapping and other cases. Textures can be 1D, 2D, 3D or cube maps.
     *
     */
    class MGEGRAPHICS_EXPORT texture : public context_object
    {
    protected:
        /**
         * @brief Create a texture.
         *
         * @param context render context
         * @param type    texture type
         */
        texture(render_context&, texture_type type);

    public:
        /**
         * @brief Destroy the texture object.
         */
        virtual ~texture() = default;

        /**
         * @brief Get the texture type.
         *
         * @return texture type
         */
        texture_type type() const;

    private:
        texture_type m_type;
    };

} // namespace mge