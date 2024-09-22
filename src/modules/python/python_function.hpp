#pragma once
#include "mge/script/script_fwd.hpp"
#include "pyobject_ref.hpp"
#include "python_fwd.hpp"


#include <string>

namespace mge::python {

    class python_function
    {
    public:
        python_function(python_context&                       context,
                        const mge::script::function_data_ref& function);
        virtual ~python_function() = default;
        void on_interpreter_loss();
        void on_interpreter_restore();

    private:
        void initialize();

        python_context& m_context;
        std::string     m_name;
        std::string     m_module_name;

        pyobject_ref                   m_object;
        mge::script::function_data_ref m_function;
    };
} // namespace mge::python