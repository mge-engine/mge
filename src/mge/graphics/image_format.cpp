// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/image_format.hpp"
#include <iostream>

namespace mge {

    std::ostream& operator<<(std::ostream& os, const image_format& f)
    {
        return os << std::format("{}", f);
    }

    size_t binary_size(image_format f)
    {
        switch (f) {
        case image_format::RGB_BYTE:
            return 3;
        case image_format::RGBA_BYTE:
            return 4;
        case image_format::RGB_FLOAT:
            return 3 * sizeof(float);
        case image_format::RGBA_FLOAT:
            return 4 * sizeof(float);
        default:
            return 0;
        }
    }

} // namespace mge