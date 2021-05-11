// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <memory>

namespace mge {

    /**
     * A render context is a context in which objects that are elements
     * of the render pipeline are created - such as shader programs,
     * textures and buffers.
     */
    class MGEGRAPHICS_EXPORT render_context
        : public std::enable_shared_from_this<render_context>
    {
    public:
        virtual ~render_context() = default;

    protected:
        render_context();
    };
} // namespace mge