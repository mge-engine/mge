// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/closure.hpp"
#include "mge/core/component.hpp"
#include "mge/script/script_context.hpp"
#include "python.hpp"
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
        python_type_ref
        find_component_type(const std::vector<PyObject*>& types);

    private:
        void bind_module(const mge::script::module_data_ref& data);
        void bind_module_functions(const mge::script::module_data_ref& data);
        void bind_helper_module();
        void create_function_helper_type(const python_module_ref& mod);
        bool is_builtin(const mge::script::type_data_ref& t) const;
        void evaluate_prelude();

        PyObject* register_component(PyObject* self, PyObject* args);
        PyObject* create_component(PyObject* self, PyObject* args);

        void
        implementations(std::string_view component_name,
                        const std::function<void(std::string_view)>& callback);

        std::shared_ptr<mge::component_base>
        create(std::string_view component_name,
               std::string_view implementation_name);

        using function_closure = mge::closure<PyObject*, PyObject*, PyObject*>;

        python_engine_ref                                     m_engine;
        std::map<mge::script::type_data_ref, python_type_ref> m_types;
        std::map<std::string, python_module_ref>              m_modules;
        std::vector<python_module_ref>                        m_all_modules;
        std::vector<python_function_ref>                      m_functions;
        std::shared_ptr<function_closure> m_register_component;
        std::shared_ptr<function_closure> m_create_component;
        std::vector<PyMethodDef>          m_methods;

        static python_context*              s_global_context;
        static thread_local python_context* s_thread_context;
        friend class python_component_registry;
    };
} // namespace mge::python