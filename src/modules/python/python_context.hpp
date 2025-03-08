// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/script_context.hpp"
#include "python_fwd.hpp"

#include <functional>
#include <map>
#include <type_traits>

namespace mge::python {
    class python_context : public mge::script_context
    {
    public:
        using restore_action = std::function<void()>;

        python_context(const python_engine_ref& engine);
        ~python_context() override;

        void eval(const std::string& code) override;
        int  main(int argc, const char** argv) override;
        void bind() override;

        void on_interpreter_loss();
        void on_interpreter_restore();

        const python_module_ref& module(const std::string& name) const;
        const python_type_ref& type(const mge::script::type_data_ref& t) const;

    private:
        void bind_module(const mge::script::module_data_ref& data);
        void bind_module_functions(const mge::script::module_data_ref& data);
        void bind_helper_module();
        void create_function_helper_type(const python_module_ref& mod);
        bool is_builtin(const mge::script::type_data_ref& t) const;
        void evaluate_prelude();

        python_engine_ref                                     m_engine;
        std::map<mge::script::type_data_ref, python_type_ref> m_types;
        std::map<std::string, python_module_ref>              m_modules;
        std::vector<python_module_ref>                        m_all_modules;
        std::vector<python_function_ref>                      m_functions;
    };
} // namespace mge::python