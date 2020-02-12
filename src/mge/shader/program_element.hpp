#pragma once
#include "mge/shader/dllexport.hpp"
#include "mge/shader/location.hpp"
namespace mge {
namespace shader {

    class MGE_SHADER_EXPORT program_element
    {
    public:
        program_element();
        explicit program_element(const location& l);
        program_element(const program_element& e);
        virtual ~program_element() = default;

        location source_location() const { return m_source_location; }
    private:
        location m_source_location;
    };
}
}
