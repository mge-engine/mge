// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/texture.hpp"
#include "mge/scene/material.hpp"

namespace mge {

    /**
     * Simple texture material with an optional albedo texture and a
     * solid color tint (default: opaque white).
     */
    class MGESCENE_EXPORT texture_material : public material
    {
    public:
        texture_material() = default;
        texture_material(const texture_material&) = default;
        texture_material(texture_material&&) = default;
        texture_material& operator=(const texture_material&) = default;
        texture_material& operator=(texture_material&&) = default;
        ~texture_material() override = default;

        material_type type() const override
        {
            return material_type::TEXTURE;
        }

        /**
         * Albedo (base color) texture. May be null.
         */
        texture_ref albedo;

        /**
         * Solid color tint applied on top of the texture.
         * Default is opaque white (1, 1, 1, 1).
         */
        rgba_color color{1.0f, 1.0f, 1.0f, 1.0f};
    };

} // namespace mge
