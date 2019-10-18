// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/image_format.hpp"
#include <iostream>
namespace mge {

    std::ostream&
    operator <<(std::ostream& os, const image_format& f)
    {
        switch(f) {
        case image_format::ABGR8:
            return os << "ABGR8";
        case image_format::A8:
            return os << "A8";
        case image_format::ARGB8:
            return os << "ARGB8";
        case image_format::BGR8:
            return os << "BGR8";
        case image_format::RGB8:
            return os << "RGB8";
        case image_format::RGB16F:
            return os << "RGB16F";
        case image_format::RGB32F:
            return os << "RGB32F";
        case image_format::RGBA8 :
            return os << "RGBA8";
        default:
            return os << "INVALID(" << (std::uint16_t)f << ")";
        }
    }

    size_t pixel_size(const image_format& f)
    {
        switch(f) {
        case image_format::ABGR8:
            return 4;
        case image_format::A8:
            return 1;
        case image_format::ARGB8:
            return 4;
        case image_format::BGR8:
            return 3;
        case image_format::RGB8:
            return 3;
        case image_format::RGB16F:
            return 6;
        case image_format::RGB32F:
            return 12;
        case image_format::RGBA8 :
            return 4;
        default:
            return 0;
        }
    }
}
