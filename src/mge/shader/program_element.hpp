#pragma once
#include "mge/shader/dllexport.hpp"
#include "mge/shader/shader_fwd.hpp"
namespace mge {
namespace shader {

    class MGE_SHADER_EXPORT program_element
    {
    public:
        program_element(mge::shader::program *p);
        virtual ~program_element() = default;
        mge::shader::program *program() const { return m_program; }
    private:
        mge::shader::program *m_program;
    };
}
}
