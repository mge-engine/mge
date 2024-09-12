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

        const python_module_ref& get_module(const std::string& name) const;

    private:
        void bind_module(const mge::script::module_data_ref& data);
        void bind_module_functions(const mge::script::module_data_ref& data);
        void bind_helper_module();
        bool is_builtin(const mge::script::type_data_ref& t) const;

        python_engine_ref                                     m_engine;
        std::map<mge::script::type_data_ref, python_type_ref> m_types;
        std::map<std::string, python_module_ref>              m_modules;
    };
} // namespace mge::python