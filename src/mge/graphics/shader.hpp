// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/core/buffer.hpp"

namespace mge {

    /**
     * A shader is a piece of code executed on the GPU.
     * The pipeline stage is determined by the shader type.
     */
    class MGE_GRAPHICS_EXPORT shader
            : context_object
    {
    protected:
        shader(render_context& context,
               shader_type type);
    public:
        virtual ~shader();
        /**
         * Compile from source code.
         * @param source_code shader source code
         */
        virtual void compile(const std::string& source_code) = 0;

        /**
         * Set compiled code.
         * @param code buffer with compiled code
         */
        virtual void load(const buffer& code) = 0;

        /**
         * Store compiled code in buffer.
         * @oaram code compiled code
         */
        virtual void save(const buffer& code) = 0;
    };
}
