// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/graphics/dllexport.hpp"
#include <iosfwd>
#include <string_view>

namespace mge {

    /**
     * @brief Attribute semantic.
     *
     * Vertex attributes carry a semantic, which describes the attribute's
     * purpose.
     */
    enum class MGEGRAPHICS_EXPORT attribute_semantic : uint8_t
    {
        POSITION,
        NORMAL,
        TANGENT,
        BITANGENT,
        COLOR,
        TEXCOORD,
        ANY
    };

    /**
     * @brief Parse data type from string view.
     *
     * @param sv string view
     * @return parsed data type
     */
    MGEGRAPHICS_EXPORT attribute_semantic
    parse_attribute_semantic(std::string_view sv);

    namespace literals {
        inline attribute_semantic operator""_semantic(const char* s, size_t l)
        {
            return parse_attribute_semantic(std::string_view(s, s + l));
        }
    } // namespace literals

} // namespace mge