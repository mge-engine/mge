// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"

namespace mge {
    namespace opengl {
        window::window(const mge::extent &        extent,
                       const mge::window_options &options)
            : platform::window(extent, options)
        {}

        window::~window() {}
    } // namespace opengl
} // namespace mge