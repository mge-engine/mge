// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/semantic_version.hpp"
#include "mge/graphics/dllexport.hpp"

#include <iosfwd>
#include <string>
#include <string_view>

namespace mge {

    /**
     * @brief Shader format.
     *
     * A shader format describes the binary format accepted by the graphics
     * engine to define shader programs. Well-known formats are HLSL and
     * SPIRV.
     */
    class MGEGRAPHICS_EXPORT shader_format
    {
    public:
        /**
         * @brief Construct an empty uninitialized format.
         */
        shader_format();

        /**
         * @brief Construct a shader format.
         *
         * @param name shader format name
         */
        shader_format(std::string_view name);

        /**
         * @brief Construct a shader format.
         *
         * @param name shader format name
         * @param version shader format version
         */
        shader_format(std::string_view name, const semantic_version& version);

        /**
         * Destructor
         */
        ~shader_format();

    private:
        std::string      m_name;
        semantic_version m_version;
    };
} // namespace mge
