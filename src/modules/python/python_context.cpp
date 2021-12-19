// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_context.hpp"
#include "mge/core/trace.hpp"
#include "python_bind_helper.hpp"
#include "python_error.hpp"
#include "python_module.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_context::python_context()
        : m_main_module(nullptr)
        , m_main_dict(nullptr)
        , m_main_dict_copy(nullptr)
        , m_function_type(nullptr)
    {
        m_main_module = PyImport_AddModule("__main__");
        m_main_dict = PyModule_GetDict(m_main_module);
        m_main_dict_copy = PyDict_Copy(m_main_dict);
    }

    python_context::~python_context()
    {
        Py_CLEAR(m_function_type);
        Py_CLEAR(m_main_dict_copy);
        Py_CLEAR(m_main_dict);
        Py_CLEAR(m_main_module);
    }

    void python_context::eval(const std::string& code)
    {
        PyObject* result = PyRun_String(code.c_str(),
                                        Py_file_input,
                                        m_main_dict_copy,
                                        m_main_dict_copy);
        Py_CLEAR(result);

        error::check_error();
    }

    int python_context::main(int argc, const char** argv)
    {
        return Py_BytesMain(argc, const_cast<char**>(argv));
    }

    void python_context::bind(const mge::script::module& m)
    {
        python_bind_helper helper(*this);
        helper.bind(m);
    }

    void python_context::add_module(const python_module_ref& pm)
    {
        m_modules[pm->full_name()] = pm;
    }

    python_module_ref
    python_context::get_module(const std::string& full_name) const
    {
        auto it = m_modules.find(full_name);
        if (it != m_modules.end()) {
            return it->second;
        } else {
            return python_module_ref();
        }
    }

    void python_context::add_type(const python_type_ref& type)
    {
        m_types[type->full_name()] = type;
    }

    void python_context::mark_visited(const void* ptr)
    {
        m_visited_objects.insert(ptr);
    }

    bool python_context::visited(const void* ptr) const
    {
        return m_visited_objects.find(ptr) != m_visited_objects.end();
    }

} // namespace mge::python