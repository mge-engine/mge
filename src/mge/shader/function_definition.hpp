#pragma once
#include "mge/shader/dllexport.hpp"
#include "mge/shader/shader_fwd.hpp"
#include "mge/shader/statement.hpp"
namespace mge {
namespace shader {

    class MGE_SHADER_EXPORT function_definition
            : public statement
    {
    public:
        function_definition(mge::shader::module *m);
        virtual ~function_definition() = default;

        statement *body() const;
    private:
        statement *m_body;
    };
}
}
