#pragma once
#include "mge/shader/dllexport.hpp"
#include "mge/shader/program_type.hpp"
#include <memory_resource>

namespace mge {
namespace shader {

    /**
     * A shader program.
     */
    class MGE_SHADER_EXPORT program
    {
    public:
        program(program_type type);
        ~program();
        program_type type() const noexcept { return m_type; }
    private:
        program_type m_type;
        std::pmr::unsynchronized_pool_resource m_program_memory;
    };

}
}
