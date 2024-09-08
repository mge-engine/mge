#pragma once
#include "pyobject_ref.hpp"
#include "python.hpp"

#include "mge/script/module_data.hpp"

#include <string>

namespace mge::python {
    class python_module
    {
    public:
        python_module(const mge::script::module_data_ref& data);
        python_module(const std::string& name);

        ~python_module() = default;

    private:
        std::string  m_name;
        pyobject_ref m_module;
    };
} // namespace mge::python