// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include "python_engine.hpp"
#include "mge/config.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"

#include "gil_lock.hpp"
#include "python.hpp"
#include "python_context.hpp"
#include "python_error.hpp"

#include <filesystem>
#include <set>

#ifdef MGE_OS_WINDOWS
#    include <windows.h>

#    include <psapi.h>
#endif

namespace mge {
    MGE_DEFINE_TRACE(PYTHON);
}

namespace mge::python {

    class python_interpreter
    {
    public:
        python_interpreter() { initialize_interpreter(); }

        ~python_interpreter() { finalize_interpreter(); }

        void add_engine(python_engine* engine)
        {
            std::lock_guard<decltype(m_mutex)> guard(m_mutex);
            MGE_DEBUG_TRACE(PYTHON) << "Adding Python engine";
            m_engines.insert(engine);
        }

        void remove_engine(python_engine* engine)
        {
            std::lock_guard<decltype(m_mutex)> guard(m_mutex);
            MGE_DEBUG_TRACE(PYTHON) << "Removing Python engine";
            m_engines.erase(engine);
        }

        void interpreter_lost()
        {
            std::lock_guard<decltype(m_mutex)> guard(m_mutex);
            MGE_DEBUG_TRACE(PYTHON) << "Global python interpreter lost";
            m_initialized = false;
            for (auto& e : m_engines) {
                e->on_interpreter_loss();
            }
        }

        bool initialized() const { return m_initialized; }

        void initialize_interpreter()
        {
            std::lock_guard<decltype(m_mutex)> guard(m_mutex);
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
            m_initialized = true;

            for (auto& e : m_engines) {
                e->on_interpreter_restore();
            }
        }

    private:
#ifdef MGE_OS_WINDOWS
        static void function_with_address_in_this_module() {}

        std::wstring compute_python_home()
        {
            HMODULE module = 0;
            if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                                   reinterpret_cast<LPCSTR>(
                                       &function_with_address_in_this_module),
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

        void finalize_interpreter()
        {
            std::lock_guard<decltype(m_mutex)> guard(m_mutex);
            if (m_initialized) {
                MGE_DEBUG_TRACE(PYTHON) << "Finalizing Python interpreter";
                gil_lock gil_guard;
                Py_Finalize();
                m_initialized = false;
                for (auto& e : m_engines) {
                    e->interpreter_lost();
                }
            } else {
                MGE_DEBUG_TRACE(PYTHON)
                    << "Python interpreter already finalized";
            }
        }

        mge::mutex               m_mutex{"python_interpreter"};
        std::wstring             m_home;
        std::set<python_engine*> m_engines;
        bool                     m_initialized{false};
    };

    mge::singleton<python_interpreter> s_interpreter;

    python_engine::python_engine()
    {
        MGE_INFO_TRACE(PYTHON) << "Creating Python script engine";
        s_interpreter->add_engine(this);
    }

    python_engine::~python_engine() { s_interpreter->remove_engine(this); }

    mge::script_context_ref python_engine::create_context()
    {
        if (!m_context) {
            m_context = std::make_shared<python_context>(
                std::dynamic_pointer_cast<python_engine>(self().lock()));
        }
        return m_context;
    }

    void python_engine::interpreter_lost()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Python engine received interpreter lost";
        s_interpreter->interpreter_lost();
    }

    void python_engine::on_interpreter_loss()
    {
        if (m_context) {
            m_context->on_interpreter_loss();
        }
    }

    void python_engine::on_interpreter_restore()
    {
        if (m_context) {
            m_context->on_interpreter_restore();
        }
    }

    void python_engine::initialize_interpreter()
    {
        s_interpreter->initialize_interpreter();
    }

    bool python_engine::interpreter_initialized() const
    {
        return s_interpreter->initialized();
    }

    MGE_REGISTER_IMPLEMENTATION(python_engine, mge::script_engine, python);
} // namespace mge::python