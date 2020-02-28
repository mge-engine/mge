#pragma once
#include "mge/shader/dllexport.hpp"
#include "mge/shader/shader_fwd.hpp"
#include "mge/shader/module.hpp"
#include "mge/shader/location.hpp"

namespace mge {
namespace shader {

    class MGE_SHADER_EXPORT module_element
            : public program_element
    {
    public:
        module_element(mge::shader::module* m,
                       uint32_t begin,
                       uint32_t end);
        module_element(mge::shader::module* m);
        module_element(mge::shader::program* p);

        ~module_element() = default;

        mge::shader::location location() const { return m_location; }
        mge::shader::location& location() { return m_location; }
    private:
        mge::shader::location m_location;
    };
}
}
