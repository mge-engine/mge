#pragma once
#include "mge/shader/dllexport.hpp"
#include "mge/shader/module_element.hpp"
namespace mge {
namespace shader {

    class MGE_SHADER_EXPORT statement
            : public module_element
    {
    public:
        statement(module *m);
        statement(module *m, uint32_t begin, uint32_t end);
        virtual ~statement() = default;
    };
}
}
