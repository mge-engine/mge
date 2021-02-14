// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {
    /**
     * @brief Rectangle dimensions.
     */
    class MGEGRAPHICS_EXPORT dimension
    {
    public:
        dimension() : width(0u), height(0u) {}

        dimension(uint32_t width_, uint32_t height_)
            : width(width_), height(height_)
        {}

        dimension(const dimension &) = default;
        dimension(dimension &&)      = default;

        dimension &operator=(const dimension &) = default;
        dimension &operator=(dimension &&) = default;

        uint32_t width;
        uint32_t height;
    };

    MGEGRAPHICS_EXPORT std::ostream &operator<<(std::ostream &   os,
                                                const dimension &d);
} // namespace mge