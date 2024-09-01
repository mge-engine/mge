// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include "python_engine.hpp"
#include "mge/config.hpp"
#include "mge/core/trace.hpp"

#include "python.hpp"
#include "python_context.hpp"
#include "python_error.hpp"

namespace mge {
    MGE_DEFINE_TRACE(PYTHON);
}

namespace mge::python {

    std::atomic<uint64_t> python_engine::s_initialized_engines{0};

    python_engine::python_engine()
    {
        MGE_INFO_TRACE(PYTHON) << "Creating Python script engine";
        if (s_initialized_engines++ == 0) {
            initialize_interpreter();
        }
    }
    python_engine::~python_engine()
    {
        if (--s_initialized_engines == 0) {
            finalize_interpreter();
        }
    }

    mge::script_context_ref python_engine::create_context()
    {
        auto result = std::make_shared<python_context>(
            std::dynamic_pointer_cast<python_engine>(self().lock()));
        return result;
    }

    void python_engine::initialize_interpreter()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Initializing Python interpreter";
        PyConfig config;
        PyConfig_InitPythonConfig(&config);

        config.parse_argv = 0;
        config.install_signal_handlers = 0;

        auto status = Py_InitializeFromConfig(&config);
        PYTHON_CHECK_STATUS(status);
    }

    void python_engine::finalize_interpreter()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Finalizing Python interpreter";
        Py_Finalize();
    }
} // namespace mge::python