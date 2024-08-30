// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "python_engine.hpp"
#include "python_context.hpp"
#include <filesystem>
#include <iostream>
#include <mutex>

#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 4996)
#endif

#ifdef MGE_OS_WINDOWS
#    include <windows.h>

#    include <psapi.h>
#endif

namespace mge {
    MGE_DEFINE_TRACE(PYTHON);
}

namespace mge::python {
    python_engine::python_engine()
    {
        MGE_INFO_TRACE(PYTHON) << "Create Python script engine";
        std::lock_guard<std::mutex> guard(s_lock);

        if (!Py_IsInitialized()) {
#ifdef MGE_OS_WINDOWS
            s_python_home = compute_python_home();
            MGE_DEBUG_TRACE(PYTHON)
                << "Setting PYTHONHOME to " << s_python_home;
            Py_SetPythonHome(s_python_home.c_str());
#else
#    error Missing port
#endif
            Py_InitializeEx(0);
        }
        ++s_initialized_engines;

        s_engines.emplace_back(this);
    }

    python_engine::~python_engine()
    {
        std::lock_guard<std::mutex> guard(s_lock);
        std::erase(s_engines, this);
        if (--s_initialized_engines == 0) {
            if (Py_IsInitialized()) {
                Py_FinalizeEx();
            }
        }
    }

    script_context_ref python_engine::create_context()
    {
        auto result = std::make_shared<python_context>(
            std::dynamic_pointer_cast<python_engine>(self().lock()));

        m_contexts.emplace_back(result);

        return result;
    }

    std::mutex& python_engine::engine_lock() { return s_lock; }

    void python_engine::interpreter_lost()
    {
        // s_lock is already locked here!
        MGE_DEBUG_TRACE(PYTHON) << "Setting PYTHONHOME to " << s_python_home;
        Py_SetPythonHome(s_python_home.c_str());
        Py_InitializeEx(0);
        for (const auto& engine : s_engines) {
            engine->signal_interpreter_lost();
        }
    }

    void python_engine::signal_interpreter_lost()
    {
        for (const auto& context : m_contexts) {
            if (!context.expired()) {
                context.lock()->interpreter_lost();
            }
        }
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

    std::mutex                  python_engine::s_lock;
    uint64_t                    python_engine::s_initialized_engines = 0;
    std::vector<python_engine*> python_engine::s_engines;
    std::wstring                python_engine::s_python_home;

    MGE_REGISTER_IMPLEMENTATION(python_engine, mge::script_engine, python);
} // namespace mge::python