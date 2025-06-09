// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/canvas.hpp"

namespace mge {

    canvas::canvas(render_context& context)
        : context_object(context)
    {}

    canvas::~canvas() {}

} // namespace mge
