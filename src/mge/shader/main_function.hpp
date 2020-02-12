#pragma once
#include "mge/shader/statement.hpp"
#include "mge/shader/shader_fwd.hpp"
namespace mge {
namespace shader {

    class main_function
            : statement
    {
    public:
        main_function();
        virtual ~main_function();

        const statement_ref body() const { return m_body; }
    private:
        statement_ref m_body;
    };

}
}
