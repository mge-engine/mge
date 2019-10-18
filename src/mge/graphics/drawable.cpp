// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/drawable.hpp"

namespace mge {

    drawable::drawable()
    {}

    drawable::~drawable()
    {}

    bool
    drawable::prepared() const
    {
        return true;
    }

    void
    drawable::prepare(render_context& context)
    {}
}