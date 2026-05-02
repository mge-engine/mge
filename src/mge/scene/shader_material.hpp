// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/graphics/texture.hpp"
#include "mge/math/mat.hpp"
#include "mge/math/vec.hpp"
#include "mge/scene/material.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <variant>

namespace mge {

    /**
     * Value type for a shader material parameter.
     * Supports scalar, vector, matrix, color and texture types.
     */
    using shader_parameter_value = std::variant<bool,
                                                int32_t,
                                                float,
                                                fvec2,
                                                fvec3,
                                                fvec4,
                                                fmat2,
                                                fmat3,
                                                fmat4,
                                                rgba_color,
                                                texture_ref>;

    /**
     * Custom shader material. Holds GLSL/HLSL/etc. source code per
     * shader stage and a named parameter (uniform) map.
     */
    class MGESCENE_EXPORT shader_material : public material
    {
    public:
        shader_material() = default;
        shader_material(const shader_material&) = default;
        shader_material(shader_material&&) = default;
        shader_material& operator=(const shader_material&) = default;
        shader_material& operator=(shader_material&&) = default;
        ~shader_material() override = default;

        material_type type() const override
        {
            return material_type::SHADER;
        }

        /**
         * Shader source code per stage (e.g., VERTEX, FRAGMENT).
         * Key = shader_type, value = source code string.
         */
        std::map<shader_type, std::string> sources;

        /**
         * Named parameter (uniform) values passed to the shader.
         * Key = parameter name, value = typed parameter value.
         */
        std::map<std::string, shader_parameter_value> parameters;
    };

} // namespace mge
