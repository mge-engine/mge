// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.

#include "python.hpp"

#include "python_context.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"

#include "mge/config.hpp"
#include "mge/core/format.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"

#ifdef MGE_OS_WINDOWS

#    include <windows.h>

#    include <psapi.h>

#endif

namespace mge {
    MGE_DEFINE_TRACE(PYTHON);
}

namespace mge::python {

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
        MGE_DEBUG_TRACE(PYTHON, "Python module name is: {}", filename);

        std::filesystem::path pythonhome(filename);
        pythonhome.replace_filename(std::filesystem::path("python3"));
        MGE_DEBUG_TRACE(PYTHON, "Python home is: {}", pythonhome.string());
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
#elif defined(MGE_OS_MACOSX) || defined(MGE_OS_LINUX)
#    include <dlfcn.h>

    static void function_with_address_in_this_module() {}

    std::wstring compute_python_home()
    {
        Dl_info info;
        if (!dladdr(
                reinterpret_cast<void*>(&function_with_address_in_this_module),
                &info) ||
            !info.dli_fname) {
            MGE_THROW(mge::system_error) << "dladdr failed";
        }
        MGE_DEBUG_TRACE(PYTHON, "Python module name is: {}", info.dli_fname);

        std::filesystem::path pythonhome(info.dli_fname);
        pythonhome.replace_filename(std::filesystem::path("python3"));
        MGE_DEBUG_TRACE(PYTHON, "Python home is: {}", pythonhome.string());
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
#    error "Unsupported platform"
#endif

    python_engine::python_engine()
        : m_home(compute_python_home())
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
        PyConfig config;
        PyConfig_InitPythonConfig(&config);

        config.parse_argv = 0;
        config.install_signal_handlers = 0;

        auto result = PyConfig_SetString(&config, &config.home, m_home.c_str());
        PYTHON_CHECK_STATUS(result);

        result = Py_InitializeFromConfig(&config);
        MGE_DEBUG_TRACE(PYTHON,
                        "Py_InitializeFromConfig returned with exit code: {}",
                        result.exitcode);
        PYTHON_CHECK_STATUS(result);

        m_main_thread_state = PyGILState_GetThisThreadState();
    }

    MGE_REGISTER_IMPLEMENTATION(python_engine, mge::script_engine, python);

} // namespace mge::python
