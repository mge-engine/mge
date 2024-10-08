// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "pyobject_ref.hpp"
#include "python.hpp"
#include "python_fwd.hpp"

#include "mge/script/module_data.hpp"

#include <string>

namespace mge::python {
    class python_module
    {
    public:
        python_module(python_context&                     context,
                      const mge::script::module_data_ref& data);
        python_module(python_context& context, const std::string& name);

        ~python_module();

        void on_interpreter_loss();
        void on_interpreter_restore();

        const pyobject_ref& pymodule() const { return m_module; }
        const auto&         data() const { return m_data; }

    private:
        void initialize();

        python_context&              m_context;
        mge::script::module_data_ref m_data;
        std::string                  m_name;
        pyobject_ref                 m_module;
    };
} // namespace mge::python