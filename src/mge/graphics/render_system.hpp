// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/core/component.hpp"

namespace mge {

    class MGE_GRAPHICS_EXPORT render_system
            : public component<render_system>
    {
    protected:
        render_system();
    public:
        virtual ~render_system();
    };
}
