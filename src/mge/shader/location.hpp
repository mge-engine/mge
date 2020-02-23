#pragma once
#include "mge/shader/dllexport.hpp"
#include "mge/shader/shader_fwd.hpp"
#include <iostream>

namespace mge {
namespace shader {

    class MGE_SHADER_EXPORT location
    {
    public:
        location();
        location(const mge::shader::module *m, uint32_t begin, uint32_t end);
        location(const mge::shader::module *m);
        location(const location& l);
        ~location();

        location& operator =(const location& l);
        location& operator =(location&& l);

        const mge::shader::module *module() const { return m_module; }
        uint32_t begin() const noexcept { return m_begin; }
        uint32_t end() const noexcept { return m_end; }

    private:
        const mge::shader::module *m_module;
        uint32_t m_begin;
        uint32_t m_end;
    };

    // MGE_SHADER_EXPORT std::ostream& operator <<(std::ostream& os, const location& l);

}
}
