// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace.hpp"
#include "mge/script/script_engine.hpp"
#include "python_context.hpp"


namespace mge {
    MGE_DEFINE_TRACE(PYTHON);
}

namespace mge::python {

    class python_engine : public script_engine
    {
    public:
        python_engine()
        {
            MGE_INFO_TRACE(PYTHON, "Python script engine created");
        }

        ~python_engine() = default;

        script_context_ref create_context() override
        {
            return std::make_shared<python_context>(this);
        }
    };

    MGE_REGISTER_IMPLEMENTATION(python_engine, mge::script_engine, python);

} // namespace mge::python
