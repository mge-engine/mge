// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/component.hpp"
#include "mge/graphics/dllexport.hpp"

namespace mge {
    /**
     * @brief Interface for shader compilers.
     *
     * A shader compiler can compile shader source code to a binary
     * format that can be uploaded to the GPU. This is used for shader
     * languages that require compilation before execution.
     */
    class MGEGRAPHICS_EXPORT shader_compiler : public component<shader_compiler>
    {
    protected:
        shader_compiler() noexcept = default;
        virtual ~shader_compiler();
    };

} // namespace mge