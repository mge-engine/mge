// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/program.hpp"

namespace mge {

    /**
     * @brief A uniform binding.
     *
     * An uniform binding captures bound uniforms of a shader program.
     * It is used to bind uniforms in shaders.
     */
    class MGEGRAPHICS_EXPORT uniform_binding
    {
    public:
        uniform_binding(const program_ref& program);
        ~uniform_binding() = default;

    private:
        program_ref m_program; //!< The shader program this binding is for.
    };
} // namespace mge