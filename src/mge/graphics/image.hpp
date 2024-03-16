// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"

namespace mge {

    /**
     * An image. Base class for image types.
     */
    class image
    {
    public:
        image() = default;
        virtual ~image() = default;
    };
}
}