// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.

#include "python.hpp"

#include "python_context.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"


#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(PYTHON);
}

namespace mge::python {

    python_engine::python_engine()
    {
        MGE_INFO_TRACE(PYTHON, "Python script engine created");
        init_main_state();
    }

    python_engine::~python_engine()
    {
        MGE_INFO_TRACE(PYTHON, "Python script engine destroyed");
    }

    script_context_ref python_engine::create_context()
    {
        return std::make_shared<python_context>(this);
    }

    void python_engine::init_main_state()
    {
        PyConfig_InitPythonConfig(&m_config);
        m_config.isolated = 1;
        m_config.use_environment = 0;
        m_config.install_signal_handlers = 0;
        m_config.configure_c_stdio = 0;

        auto result = Py_InitializeFromConfig(&m_config);
        MGE_DEBUG_TRACE(PYTHON,
                        "Py_InitializeFromConfig returned with exit code: {}",
                        result.exitcode);
        PYTHON_CHECK_STATUS(result);

        m_main_thread_state = PyGILState_GetThisThreadState();
    }

    MGE_REGISTER_IMPLEMENTATION(python_engine, mge::script_engine, python);

} // namespace mge::python
