// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/script_engine.hpp"

namespace mge::python {

    class python_engine : public mge::script_engine
    {
    public:
        python_engine();
        ~python_engine() override;

        script_context_ref create_context() override;
    };

} // namespace mge::python
