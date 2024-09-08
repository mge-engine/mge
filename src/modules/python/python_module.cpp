#include "python_module.hpp"
#include "python_error.hpp"

namespace mge::python {

    python_module::python_module(const mge::script::module_data_ref& data)
    {
        PyObject* module = PyImport_AddModule(data->full_name().c_str());
        error::check_error();
        if (!module) {
            MGE_THROW(python::error)
                << "Cannot create module " << data->full_name();
        }
        std::string parent_name;
        if (data->parent()->is_root()) {
            parent_name = "__main__";
        } else {
            parent_name = data->parent()->full_name();
        }
        pyobject_ref parent_module(PyImport_ImportModule(parent_name.c_str()));
        error::check_error();
        if (!parent_module) {
            MGE_THROW(python::error)
                << "Cannot import parent module " << parent_name;
        }
        PyObject* parent_dict = PyModule_GetDict(parent_module.get());
        error::check_error();
        if (!parent_dict) {
            MGE_THROW(python::error)
                << "Cannot get dictionary of parent module " << parent_name;
        }
        PyDict_SetItemString(parent_dict, data->name().c_str(), module);
        error::check_error();
        m_module = pyobject_ref(module, pyobject_ref::incref::yes);
    }

    python_module::python_module(const std::string& name) {}
} // namespace mge::python