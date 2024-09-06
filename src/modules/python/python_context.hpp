#pragma once
#include "bind_step.hpp"
#include "python_fwd.hpp"

#include "mge/script/script_context.hpp"

namespace mge::python {
    class python_context : public mge::script_context
    {
    public:
        python_context(const python_engine_ref& engine);
        ~python_context() override;

        void eval(const std::string& code) override;
        int  main(int argc, const char** argv) override;
        void bind() override;

    private:
        void create_steps_for_module(const mge::script::module_data_ref& module,
                                     std::vector<bind_step_ref>&         steps);
        python_engine_ref m_engine;
    };
} // namespace mge::python