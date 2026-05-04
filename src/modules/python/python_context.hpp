// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/script_context.hpp"

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
        python_engine* m_engine;
    };

} // namespace mge::python
