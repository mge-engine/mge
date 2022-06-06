// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_context.hpp"
#include "mge/core/contains.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "python_binder.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"

#include <iostream>

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
        PyGILState_STATE gil_state;
        gil_state = PyGILState_Ensure();
        PyObject* main_module = nullptr;
        PyObject* main_dict = nullptr;
        try {
            main_module = PyImport_AddModule("__main__");
            error::check_error();
            main_dict = PyModule_GetDict(main_module);
            error::check_error();
            PyObject* result =
                PyRun_String(code.c_str(), Py_file_input, main_dict, main_dict);
            Py_CLEAR(result);
            error::check_error();
            Py_CLEAR(main_dict);
            Py_CLEAR(main_module);
            PyGILState_Release(gil_state);
        } catch (...) {
            Py_CLEAR(main_dict);
            Py_CLEAR(main_module);
            PyGILState_Release(gil_state);
            throw;
        }
    }

    void python_context::bind(const mge::script::module& m)
    {
        PyGILState_STATE gil_state;
        gil_state = PyGILState_Ensure();
        try {
            python_binder b(*this);
            b.bind(m);
            PyGILState_Release(gil_state);
        } catch (...) {
            PyGILState_Release(gil_state);
            throw;
        }
        m_rebind_functions.emplace_back([this, m]() {
            PyGILState_STATE gil_state;
            gil_state = PyGILState_Ensure();
            try {
                python_binder b(*this);
                b.bind(m);
                PyGILState_Release(gil_state);
            } catch (...) {
                PyGILState_Release(gil_state);
                throw;
            }
        });
    }

    int python_context::main(int argc, const char** argv)
    {
        int rc = Py_BytesMain(argc, const_cast<char**>(argv));
        {
            std::lock_guard<std::mutex> guard(python_engine::engine_lock());
            if (!Py_IsInitialized()) {
                python_engine::interpreter_lost();
            }
        }
        return rc;
    }

    python_module_ref
    python_context::get_or_add_module(const mge::script::module& m)
    {
        auto it = m_python_modules.find(m);
        if (it != m_python_modules.end()) {
            return it->second;
        } else {
            if (m.is_root()) {
                auto mod = std::make_shared<python_module>(*this, m);
                return m_python_modules[m] = mod;
            } else {
                auto parent = get_or_add_module(m.parent());
                auto mod = std::make_shared<python_module>(*this, parent, m);
                return m_python_modules[m] = mod;
            }
        }
    }

    python_module_ref
    python_context::get_module(const mge::script::module& m) const
    {
        auto it = m_python_modules.find(m);
        if (it == m_python_modules.end()) {
            MGE_THROW(mge::no_such_element) << "No such element: " << m.name();
        }
        return it->second;
    }

    void python_context::rebuild()
    {
        for (const auto& [t, type] : m_python_types) {
            type->interpreter_lost();
        }
        for (const auto& [m, mod] : m_python_modules) {
            mod->interpreter_lost();
        }
        m_python_types.clear();
        m_python_modules.clear();
        for (auto& f : m_rebind_functions) {
            f();
        }
    }

} // namespace mge::python
