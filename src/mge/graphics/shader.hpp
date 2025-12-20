// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/buffer.hpp"
#include "mge/core/property_object.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/shader_type.hpp"
#include <string_view>
#include <variant>

namespace mge {
    /**
     * @brief A shader is a piece of code executed in the graphics pipeline.
     */
    class MGEGRAPHICS_EXPORT shader : public context_object,
                                      public property_object
    {
    protected:
        /**
         * @brief Construct a new shader object.
         *
         * @param context render context of shader
         * @param type    shader type
         */
        shader(render_context& context, shader_type type);

    public:
        /**
         * @brief Compiles shader from source.
         *
         * @param source source code
         */
        void compile(std::string_view source);

        /**
         * @brief Sets shader by uploading compiled code.
         *
         * @param code compiled code
         */
        void set_code(const mge::buffer& code);

        /**
         * @brief Get shader type.
         *
         * @return shader type
         */
        shader_type type() const;

        /**
         * @brief Returns whether shader contains executable code.
         *
         * @return @c true if shader is executable, either by upload or by
         * compilation
         */
        bool initialized() const;

        /**
         * @brief Destroy the shader.
         *
         * The shader is removed from the context and
         * its resources are freed. Afterwards, the pointer to the
         * shader is invalid.
         */
        void destroy();

        /**
         * @brief Destructor.
         */
        virtual ~shader() = default;

    protected:
        virtual void on_compile(std::string_view source) = 0;
        virtual void on_set_code(const mge::buffer& code) = 0;

        shader_type m_type;
        bool        m_initialized;
    };
} // namespace mge