#include "python_module.hpp"
#include "gil_lock.hpp"
#include "mge/core/trace.hpp"
#include "python_context.hpp"
#include "python_error.hpp"

#include <mutex>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_module::python_module(python_context&                     context,
                                 const mge::script::module_data_ref& data)
        : m_context(context)
        , m_data(data)
        , m_name(data->full_name())
    {
        MGE_DEBUG_TRACE(PYTHON) << "Creating module " << m_name;
        auto setup = [this](void) {
            std::lock_guard<gil_lock> lock(gil_lock::instance());
            PyObject* module = PyImport_AddModule(m_name.c_str());
            error::check_error();
            if (!module) {
                MGE_THROW(python::error) << "Cannot create module " << m_name;
            }
            std::string parent_name;
            if (m_data->parent()->is_root()) {
                parent_name = "__main__";
            } else {
                parent_name = m_data->parent()->full_name();
            }
            pyobject_ref parent_module(
                PyImport_ImportModule(parent_name.c_str()));
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
            PyDict_SetItemString(parent_dict, m_data->name().c_str(), module);
            error::check_error();
            m_module = pyobject_ref(module, pyobject_ref::incref::yes);
        };
        setup();
        auto restore = [=, this](void) {
            m_module.reset();
            setup();
        };
        m_restore_token = m_context.add_restore_action(restore);
    }

    python_module::python_module(python_context&    context,
                                 const std::string& name)
        : m_context(context)
        , m_name(name)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Creating module " << name;
        auto setup = [this]() {
            std::lock_guard<gil_lock> lock(gil_lock::instance());
            PyObject* module = PyImport_AddModule(m_name.c_str());
            error::check_error();
            if (!module) {
                MGE_THROW(python::error) << "Cannot create module " << m_name;
            }
            m_module = pyobject_ref(module, pyobject_ref::incref::yes);
            std::string  parent_name = "__main__";
            pyobject_ref parent_module(
                PyImport_ImportModule(parent_name.c_str()));
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
            PyDict_SetItemString(parent_dict, m_name.c_str(), module);
            error::check_error();
            m_module = pyobject_ref(module, pyobject_ref::incref::yes);
        };
        setup();
        auto restore = [=, this](void) {
            m_module.reset();
            setup();
        };
        m_restore_token = m_context.add_restore_action(restore);
    }

    python_module::~python_module()
    {
        m_context.remove_restore_action(m_restore_token);
        MGE_DEBUG_TRACE(PYTHON) << "Destroying module " << m_name;
    }
} // namespace mge::python