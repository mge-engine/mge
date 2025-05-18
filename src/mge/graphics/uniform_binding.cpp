// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/uniform_binding.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    uniform_binding::uniform_binding(const program_ref& program)
        : m_program(program)
    {
        if (!m_program) {
            MGE_THROW(illegal_argument)
                << "Program must not be invalid refernce";
        }
        if (m_program->needs_link()) {
            MGE_THROW(illegal_state)
                << "Program must be linked before binding uniforms";
        }
    }

} // namespace mge