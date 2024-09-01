// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include "python_engine.hpp"
#include "mge/config.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"

#include "python.hpp"
#include "python_context.hpp"
#include "python_error.hpp"

#include <filesystem>

#ifdef MGE_OS_WINDOWS
#    include <windows.h>

#    include <psapi.h>
#endif

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
    }

    void python_engine::finalize_interpreter()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Finalizing Python interpreter";
        Py_Finalize();
    }

#ifdef MGE_OS_WINDOWS
    void python_engine::function_with_address_in_this_module() {}

    std::wstring python_engine::compute_python_home()
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

    MGE_REGISTER_IMPLEMENTATION(python_engine, mge::script_engine, python);
} // namespace mge::python