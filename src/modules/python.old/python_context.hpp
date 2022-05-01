// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/script_context.hpp"
#include "python.hpp"
#include "python_module.hpp"
#include "python_type.hpp"

#include <map>
#include <set>

namespace mge::python {
    class python_context : public script_context
    {
    public:
        python_context();
        ~python_context();
        void eval(const std::string& code) override;
        void bind(const mge::script::module& m) override;
        int  main(int argc, const char** argv) override;

        void              add_module(const python_module_ref& pm);
        python_module_ref get_module(const std::string& full_name) const;
        void              add_type(const python_type_ref& type);

        void mark_visited(const void* ptr);
        bool visited(const void* ptr) const;

        PyObject* function_type() const { return m_function_type; }
        void set_function_type(PyObject* object) { m_function_type = object; }

    private:
        PyObject* m_main_module;
        PyObject* m_main_dict;
        PyObject* m_main_dict_copy;
        PyObject* m_function_type;

        std::map<std::string, python_type_ref>   m_types;
        std::map<std::string, python_module_ref> m_modules;
        std::set<const void*>                    m_visited_objects;
    };
} // namespace mge::python