// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/buffer.hpp"
#include "mge/core/property_object.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/shader_type.hpp"

#include <string>

namespace mge {

    /**
     * A shader program is a piece of code executed on the GPU.
     * The pipeline stage is determined by the shader type.
     */
    class MGE_GRAPHICS_EXPORT shader_program : public context_object,
                                               public property_object
    {
    protected:
        shader_program(render_context &context, shader_type type);

    public:
        virtual ~shader_program();
        /**
         * Compile from source code.
         * @param source_code shader source code
         */
        void compile(const std::string &source_code);

        /**
         * Set compiled code.
         * @param code buffer with compiled code
         */
        void load(const buffer &code);

        /**
         * Get shader type.
         * @return shader type
         */
        shader_type type() const noexcept { return m_type; }

        /**
         * Returns whether the shader has code set. Code in the shader can
         * be set by compiling code, or by loading code from binary.
         *
         * @return @c true if either @c compile or @c load have been used
         *   to define the shader
         */
        bool defined() const noexcept { return m_defined; }

    protected:
        virtual void on_compile(const std::string &source_code) = 0;
        virtual void on_load(const mge::buffer &code)           = 0;

        shader_type m_type;
        bool        m_defined;
    };
} // namespace mge
