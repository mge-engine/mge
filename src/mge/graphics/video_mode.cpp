// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/video_mode.hpp"

#include <iostream>

namespace mge {

    std::ostream &operator<<(std::ostream &os, const video_mode &m)
    {
        os << m.extent.width << "x" << m.extent.height;
        if (m.refresh_rate == video_mode::ANY_REFRESH_RATE) {
            os << "@ANY";
        } else {
            os << "@" << m.refresh_rate;
        }
        return os;
    }
} // namespace mge