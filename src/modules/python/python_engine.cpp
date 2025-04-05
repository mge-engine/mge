// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include "python_engine.hpp"
#include "gil_lock.hpp"
#include "python.hpp"
#include "python_context.hpp"
#include "python_error.hpp"
#include "python_interpreter.hpp"

#include "mge/config.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"

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

    python_engine::python_engine()
    {
        MGE_INFO_TRACE(PYTHON) << "Creating Python script engine";
    }

    python_engine::~python_engine()
    {
        MGE_INFO_TRACE(PYTHON) << "Destroying Python script engine";
    }

    mge::script_context_ref python_engine::create_context()
    {
        return python_interpreter::instance().context_ref();
    }

    MGE_REGISTER_IMPLEMENTATION(python_engine, mge::script_engine, python);
} // namespace mge::python