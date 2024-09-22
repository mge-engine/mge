#include "python_function.hpp"
#include "mge/script/function_data.hpp"
#include "mge/script/module_data.hpp"

namespace mge::python {

    python_function::python_function(
        python_context& context, const mge::script::function_data_ref& function)
        : m_context(context)
        , m_function(function)
    {
        m_name = m_function->name();
        m_module_name = m_function->module().lock()->full_name();
    }

    void python_function::on_interpreter_loss() { m_object.release(); }

    void python_function::on_interpreter_restore() { initialize(); }

    void python_function::initialize() {}

} // namespace mge::python