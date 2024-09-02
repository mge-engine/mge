#include "bind_step.hpp"
#include "mge/core/trace.hpp"
#include "pyobject_ref.hpp"
namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    bind_step_module::bind_step_module(const mge::script::module_data_ref& data)
        : m_data(data)
    {
        if (!m_data->is_root()) {
            m_dependencies.push_back(m_data->parent());
        }
    }

    void bind_step_module::execute()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Binding module " << m_data->name();
        // borrowed reference
        PyObject* module(PyImport_AddModule(m_data->full_name().c_str()));
        error::check_error();
        if (!module) {
            MGE_THROW(python::error)
                << "Cannot create module " << m_data->full_name();
        }
        auto parent_name = m_data->parent()->full_name();
        if (parent_name.empty()) {
            parent_name = "__main__";
        }
        MGE_DEBUG_TRACE(PYTHON) << "Parent name: " << parent_name;
        pyobject_ref parent_module(PyImport_ImportModule(parent_name.c_str()));
        error::check_error();
        if (!parent_module) {
            MGE_THROW(python::error)
                << "Cannot import parent module " << parent_name;
        }
        // borrowed reference
        PyObject* parent_dict = PyModule_GetDict(parent_module.get());
        error::check_error();
        if (!parent_dict) {
            MGE_THROW(python::error)
                << "Cannot get dictionary of parent module " << parent_name;
        }
        PyDict_SetItemString(parent_dict, m_data->name().c_str(), module);
        error::check_error();
    }

    std::span<bind_step::dependency> bind_step_module::dependencies() const
    {
        return const_cast<std::vector<bind_step::dependency>&>(m_dependencies);
    }

    bind_step_function::bind_step_function(
        const mge::script::module_data_ref&   module,
        const mge::script::function_data_ref& data)
        : m_module(module)
        , m_data(data)
    {}

    void bind_step_function::execute() {}

    std::span<bind_step::dependency> bind_step_function::dependencies() const
    {
        return const_cast<std::vector<bind_step::dependency>&>(m_dependencies);
    }

} // namespace mge::python