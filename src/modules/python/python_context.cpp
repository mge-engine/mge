#include "python_context.hpp"
#include "gil_lock.hpp"
#include "pyobject_ref.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"
#include "python_module.hpp"

#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_data.hpp"
#include "mge/script/type_data.hpp"
#include <mutex>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_context::python_context(const python_engine_ref& engine)
        : m_engine(engine)
    {}

    python_context::~python_context() {}

    void python_context::eval(const std::string& code)
    {
        std::lock_guard<gil_lock> lock(gil_lock::instance());
        pyobject_ref              main_module(PyImport_AddModule("__main__"));
        error::check_error();
        if (!main_module) {
            MGE_THROW(python::error) << "Cannot get main module";
        }
        pyobject_ref main_dict(PyModule_GetDict(main_module.get()));
        error::check_error();
        if (!main_dict) {
            MGE_THROW(python::error) << "Cannot get main module dictionary";
        }
        pyobject_ref result(PyRun_String(code.c_str(),
                                         Py_file_input,
                                         main_dict.get(),
                                         main_dict.get()));
        error::check_error();
    }

    int python_context::main(int argc, const char** argv)
    {
        int rc = Py_BytesMain(argc, const_cast<char**>(argv));
        MGE_DEBUG_TRACE(PYTHON) << "Python main returned " << rc;
        return rc;
    }

    bool python_context::is_builtin(const mge::script::type_data_ref& t) const
    {
        return mge::script::dependency::builtin_dependencies().find(t) !=
               mge::script::dependency::builtin_dependencies().end();
    }

    void python_context::bind()
    {
        mge::script::module root = mge::script::module::root();

        bind_helper_module();

        for (const auto& m : root.data()->modules()) {
            bind_module(m);
        }
#if 0        
        for (const auto& t : mge::script::type_data::all()) {
            MGE_DEBUG_TRACE(PYTHON) << "Creating type " << t->name();
            if (is_builtin(t)) {
                MGE_DEBUG_TRACE(PYTHON)
                    << "Type " << t->name() << " is builtin";
                continue;
            }
            python_type_ref pt = std::make_shared<python_type>(t);
            m_types[t] = pt;
        }
#endif
    }

    void python_context::bind_module(const mge::script::module_data_ref& data)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Binding module " << data->full_name();
        PyObject* module(PyImport_AddModule(data->full_name().c_str()));
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
        for (const auto& m : data->modules()) {
            bind_module(m);
        }
    }

    void python_context::bind_helper_module()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Binding helper module __mge__";
        PyObject* module(PyImport_AddModule("__mge__"));
        error::check_error();
        if (!module) {
            MGE_THROW(python::error) << "Cannot create module __mge__";
        }
        pyobject_ref parent_module(PyImport_ImportModule("__main__"));
        error::check_error();
        if (!parent_module) {
            MGE_THROW(python::error) << "Cannot import parent module __main__";
        }
        PyObject* parent_dict = PyModule_GetDict(parent_module.get());
        error::check_error();
        if (!parent_dict) {
            MGE_THROW(python::error)
                << "Cannot get dictionary of parent module __main__";
        }
        PyDict_SetItemString(parent_dict, "__mge__", module);
        error::check_error();
    }

} // namespace mge::python