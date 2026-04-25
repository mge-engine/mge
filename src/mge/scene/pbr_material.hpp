// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/texture.hpp"
#include "mge/scene/material.hpp"

namespace mge {

    /**
     * Physically-based rendering material following the glTF 2.0
     * metallic-roughness model.
     */
    class MGESCENE_EXPORT pbr_material : public material
    {
    public:
        pbr_material() = default;
        pbr_material(const pbr_material&) = default;
        pbr_material(pbr_material&&) = default;
        pbr_material& operator=(const pbr_material&) = default;
        pbr_material& operator=(pbr_material&&) = default;
        ~pbr_material() override = default;

        material_type type() const override
        {
            return material_type::PBR;
        }

        /**
         * Base color factor. Multiplied with the albedo texture sample.
         * Default is opaque white (1, 1, 1, 1).
         */
        rgba_color albedo_factor{1.0f, 1.0f, 1.0f, 1.0f};

        /**
         * Metallic factor in [0, 1]. 0 = dielectric, 1 = metal.
         * Default is 0.
         */
        float metallic{0.0f};

        /**
         * Roughness factor in [0, 1]. 0 = smooth, 1 = fully rough.
         * Default is 0.5.
         */
        float roughness{0.5f};

        /**
         * Base color (albedo) texture. May be null.
         */
        texture_ref albedo_texture;

        /**
         * Combined metallic-roughness texture. Green channel = roughness,
         * blue channel = metallic. May be null.
         */
        texture_ref metallic_roughness_texture;

        /**
         * Normal map texture (tangent space). May be null.
         */
        texture_ref normal_texture;

        /**
         * Ambient occlusion texture. Red channel = occlusion. May be null.
         */
        texture_ref occlusion_texture;

        /**
         * Emissive color factor. Default is black (no emission).
         */
        rgba_color emissive_factor{0.0f, 0.0f, 0.0f, 0.0f};

        /**
         * Emissive texture. May be null.
         */
        texture_ref emissive_texture;
    };

} // namespace mge
