// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/shader_type.hpp"

namespace mge {
    class MGE_GRAPHICS_EXPORT shader
            : context_object
    {
    protected:
        shader(render_context& context,
               shader_type type);
    public:
        virtual ~shader();
    };
}
