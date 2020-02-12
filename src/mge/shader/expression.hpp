#pragma once
#include "mge/shader/dllexport.hpp"
#include "mge/shader/program_element.hpp"
namespace mge {
namespace shader {

    class MGE_SHADER_EXPORT expression
            : public program_element
    {
    public:
        virtual ~expression() = default;
    protected:
        expression() = default;
    };

}
}
