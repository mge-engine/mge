// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace.hpp"
#include "mge/script/script_engine.hpp"
#include "python.hpp"
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
            MGE_INFO_TRACE(PYTHON) << "Create Python script engine";
            if (!Py_IsInitialized()) {
                Py_Initialize();
            }
        }
        virtual ~python_engine() { Py_Finalize(); }

        script_context_ref create_context()
        {
            script_context_ref result = std::make_shared<python_context>();
            return result;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(python_engine, mge::script_engine, python);
} // namespace mge::python