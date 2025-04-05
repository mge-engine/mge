// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "python_interpreter.hpp"
#include "gil_lock.hpp"
#include "python_context.hpp"
#include "python_error.hpp"

#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"

#include <filesystem>

#ifdef MGE_OS_WINDOWS
#    include <windows.h>

#    include <psapi.h>
#endif

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {
    mge::singleton<python_interpreter> python_interpreter::s_interpreter;

#ifdef MGE_OS_WINDOWS
    static void function_with_address_in_this_module() {}

    std::wstring compute_python_home()
    {
        HMODULE module = 0;
        if (!GetModuleHandleEx(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                reinterpret_cast<LPCSTR>(&function_with_address_in_this_module),
                &module)) {
            MGE_THROW(mge::system_error)
                << MGE_CALLED_FUNCTION(GetModuleHandleEx);
        }

        char filename[4096];
        if (!GetModuleFileNameEx(GetCurrentProcess(),
                                 module,
                                 filename,
                                 sizeof(filename))) {
            MGE_CHECK_SYSTEM_ERROR(GetModuleFileNameEx);
        }
        MGE_DEBUG_TRACE(PYTHON) << "Python module name is: " << filename;

        std::filesystem::path pythonhome(filename);
        pythonhome.replace_filename(std::filesystem::path("python3"));
        MGE_DEBUG_TRACE(PYTHON) << "Python home is: " << pythonhome;
        if (!std::filesystem::exists(pythonhome)) {
            MGE_THROW(mge::illegal_state)
                << "Installation error, python home at " << pythonhome
                << " does not exist";
        }
        if (!std::filesystem::is_directory(pythonhome)) {
            MGE_THROW(mge::illegal_state)
                << "Installation error, python home at " << pythonhome
                << " is not a directory";
        }
        return pythonhome.wstring();
    }
#else
#    error missing port
#endif

    python_interpreter::python_interpreter()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Creating Python interpreter";
        PyConfig config;
        PyConfig_InitPythonConfig(&config);

        m_home = compute_python_home();

        config.parse_argv = 0;
        config.install_signal_handlers = 0;

        PyStatus status =
            PyConfig_SetString(&config, &config.home, m_home.c_str());
        PYTHON_CHECK_STATUS(status);

        status = Py_InitializeFromConfig(&config);
        MGE_DEBUG_TRACE(PYTHON)
            << "Python initialization status: " << status.exitcode;
        PyConfig_Clear(&config);
        PYTHON_CHECK_STATUS(status);
        m_initialized = true;
    }

    void python_interpreter::interpreter_lost()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Python interpreter lost";
        m_initialized = false;
    }

    python_interpreter::~python_interpreter()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Destroying Python interpreter";
        m_context.reset();
        if (m_initialized) {
            PyGILState_Ensure();
            Py_Finalize();
            m_initialized = false;
        }
    }

    void python_interpreter::ensure_context() const
    {
        if (!m_context) {
            m_context = std::make_shared<python_context>();
        }
    }
} // namespace mge::python