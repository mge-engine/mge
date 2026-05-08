// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/script_context.hpp"
#include "python.hpp"

namespace mge::python {

    class python_engine;

    class python_context : public mge::script_context
    {
    public:
        python_context(python_engine* engine);
        ~python_context();

        void eval(const std::string& script) override;
        int  main(int argc, const char** argv) override;
        void bind() override;

    private:
        void init_interpreter();
        void create_helper_module();

        python_engine* m_engine;
        PyThreadState* m_thread_state{nullptr};
        PyObject*      m_mge_module{nullptr};
    };

} // namespace mge::python
