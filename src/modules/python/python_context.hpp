// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/script_context.hpp"
#include "python.hpp"
#include "python_module.hpp"

#include <unordered_map>

namespace mge::python {
    class python_context : public script_context
    {
    public:
        python_context();
        ~python_context();

        void eval(const std::string& code) override;
        void bind(const mge::script::module& m) override;
        int  main(int argc, const char** argv) override;

        python_module_ref get_or_add_module(const mge::script::module& m);

        PyObject* main_dict() const { return m_main_dict_copy; }

    private:
        PyObject* m_main_module;
        PyObject* m_main_dict_copy;

        std::unordered_map<mge::script::module, python_module_ref>
            m_python_modules;
    };
} // namespace mge::python