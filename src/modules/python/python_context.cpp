// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "python_context.hpp"
#include "gil_lock.hpp"
#include "pyobject_ref.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"
#include "python_module.hpp"
#include "python_type.hpp"

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
        if (!m_engine->interpreter_initialized()) {
            m_engine->initialize_interpreter();
        }
        std::lock_guard<gil_lock> lock(gil_lock::instance());
        PyObject*                 main_module = PyImport_AddModule("__main__");
        PyObject*                 global_dict = PyModule_GetDict(main_module);
        pyobject_ref              local_dict(PyDict_New());
        pyobject_ref              result(PyRun_String(code.c_str(),
                                         Py_file_input,
                                         global_dict,
                                         local_dict.get()));
        if (!result) {
            error::check_error();
        }
    }

    void interpreter_lost()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Py_AtExit handler, intepreter lost";
        python_engine::interpreter_lost();
    }

    int python_context::main(int argc, const char** argv)
    {
        int rc = 0;
        {
            PyGILState_STATE gstate;
            gstate = PyGILState_Ensure();
            Py_AtExit(interpreter_lost);
            rc = Py_BytesMain(argc, const_cast<char**>(argv));
            // no release as interpreter thread state already lost
        }
        MGE_DEBUG_TRACE(PYTHON) << "Python main returned " << rc;
        return rc;
    }

    bool python_context::is_builtin(const mge::script::type_data_ref& t) const
    {
        return mge::script::dependency::builtin_dependencies().find(t) !=
               mge::script::dependency::builtin_dependencies().end();
    }

    const python_module_ref&
    python_context::get_module(const std::string& name) const
    {
        auto it = m_modules.find(name);
        if (it == m_modules.end()) {
            MGE_THROW(python::error) << "Module " << name << " not found";
        }
        return it->second;
    }

    void python_context::bind()
    {
        mge::script::module root = mge::script::module::root();

        bind_helper_module();
        for (const auto& m : root.data()->modules()) {
            bind_module(m);
        }
        for (const auto& [id, t] : mge::script::type_data::all()) {
            MGE_DEBUG_TRACE(PYTHON) << "Creating type " << t->name();
            if (is_builtin(t)) {
                MGE_DEBUG_TRACE(PYTHON)
                    << "Type " << t->name() << " is builtin";
                continue;
            }
            python_type_ref pt = std::make_shared<python_type>(*this, t);
            m_types[t] = pt;
        }
        for (const auto& m : root.data()->modules()) {
            bind_module_functions(m);
        }
        for (auto& [t, pt] : m_types) {
            MGE_DEBUG_TRACE(PYTHON) << "Defining type " << t->name();
            pt->define_in_interpreter();
        }
    }

    void python_context::bind_module_functions(
        const mge::script::module_data_ref& data)
    {
        for (const auto& f : data->functions()) {
            MGE_DEBUG_TRACE(PYTHON) << "Binding function " << f->name();
        }
        for (const auto& m : data->modules()) {
            bind_module_functions(m);
        }
    }

    void python_context::bind_module(const mge::script::module_data_ref& data)
    {
        auto mod = std::make_shared<python_module>(*this, data);
        m_modules[data->full_name()] = mod;
        m_all_modules.push_back(mod);
        for (const auto& m : data->modules()) {
            bind_module(m);
        }
    }

    void python_context::bind_helper_module()
    {
        auto mod = std::make_shared<python_module>(*this, "__mge__");
        m_modules["__mge__"] = mod;
        m_all_modules.push_back(mod);
        create_function_helper_type(mod);
    }

    void
    python_context::create_function_helper_type(const python_module_ref& mod)
    {}

    void python_context::on_interpreter_loss()
    {
        for (auto& m : m_all_modules) {
            m->on_interpreter_loss();
        }
        for (auto& t : m_types) {
            t.second->on_interpreter_loss();
        }
    }

    void python_context::on_interpreter_restore()
    {
        for (auto& m : m_all_modules) {
            m->on_interpreter_restore();
        }
        // restore function type
        for (auto& t : m_types) {
            t.second->on_interpreter_restore();
        }
    }

} // namespace mge::python