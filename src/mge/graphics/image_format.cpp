// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/image_format.hpp"
#include "mge/core/stdexceptions.hpp"
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

    std::function<void (float& red, float& green, float& blue, float& alpha, void *pixel)>
    pixel_extractor(const image_format &f)
    {
        switch(f) {
        case image_format::A8:
            return [](float& red, float& green, float& blue, float& alpha, void *pixel)
            {
                uint8_t *data = reinterpret_cast<uint8_t *>(pixel);
                red   = 1.0f;
                green = 1.0f;
                blue  = 1.0f;
                alpha = (float)data[0] / 255.0f;
            };
        case image_format::BGR8:
            return [](float& red, float& green, float& blue, float& alpha, void *pixel)
            {
                uint8_t *data = reinterpret_cast<uint8_t *>(pixel);
                red   = (float)data[2] / 255.0f;
                green = (float)data[1] / 255.0f;
                blue  = (float)data[0] / 255.0f;
                alpha = 1.0f;
            };
        case image_format::ARGB8:
            return [](float& red, float& green, float& blue, float& alpha, void *pixel)
            {
                uint8_t *data = reinterpret_cast<uint8_t *>(pixel);
                red   = (float)data[1] / 255.0f;
                green = (float)data[2] / 255.0f;
                blue  = (float)data[3] / 255.0f;
                alpha = (float)data[0] / 255.0f;
            };
        case image_format::RGBA8:
            return [](float& red, float& green, float& blue, float& alpha, void *pixel)
            {
                uint8_t *data = reinterpret_cast<uint8_t *>(pixel);
                red   = (float)data[0] / 255.0f;
                green = (float)data[1] / 255.0f;
                blue  = (float)data[2] / 255.0f;
                alpha = (float)data[3] / 255.0f;
            };
        case image_format::RGB8:
            return [](float& red, float& green, float& blue, float& alpha, void *pixel)
            {
                uint8_t *data = reinterpret_cast<uint8_t *>(pixel);
                red   = (float)data[0] / 255.0f;
                green = (float)data[1] / 255.0f;
                blue  = (float)data[2] / 255.0f;
                alpha = 1.0f;
            };

        case image_format::RGB16F:
            return [](float& red, float& green, float& blue, float& alpha, void *pixel)
            {
                float*data = reinterpret_cast<float *>(pixel);
                red   = data[0];
                green = data[1];
                blue  = data[2];
                alpha = 1.0f;
            };
        case image_format::RGB32F:
            return [](float& red, float& green, float& blue, float& alpha, void *pixel)
            {
                double *data = reinterpret_cast<double *>(pixel);
                red   = (float)data[0];
                green = (float)data[1];
                blue  = (float)data[2];
                alpha = 1.0f;
            };
        default:
            MGE_THROW(illegal_argument) << "Invalid image format " << f;
        }
    }

}
