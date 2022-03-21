// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/script_engine.hpp"
#include "python.hpp"

namespace mge {
    MGE_DEFINE_TRACE(PYTHON);
}

namespace mge::python {
    class python_engine : public script_engine
    {
    public:
        python_engine()
        {
#ifdef MGE_OS_WINDOWS
            auto home = compute_python_home();
            MGE_DEBUG_TRACE(PYTHON) << "Setting PYTHONHOME to " << home;
            Py_SetPythonHome(home.c_str());
#else
#    error Missing port
#endif
            MGE_INFO_TRACE(PYTHON) << "Create Python script engine";
            if (!Py_IsInitialized()) {
                Py_Initialize();
            }
        }

        virtual ~python_engine() {}

        script_context_ref create_context()
        {
            script_context_ref result;
            return result;
        }

    private:
#ifdef MGE_OS_WINDOWS
        std::wstring compute_python_home()
        {
            HMODULE module = 0;
            if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                                   &s_dummy,
                                   &module)) {
                MGE_THROW(mge::system_error)
                    << MGE_CALLED_FUNCTION(GetModuleHandleEx);
            }

            char filename[4096];
            GetModuleFileNameEx(GetCurrentProcess(),
                                module,
                                filename,
                                sizeof(filename));
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
#endif
    };

    MGE_REGISTER_IMPLEMENTATION(python_engine, mge::script_engine, python);
} // namespace mge::python