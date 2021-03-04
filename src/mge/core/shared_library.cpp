// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/shared_library.hpp"
#include "mge/core/system_error.hpp"
#include <windows.h>

namespace mge {
    shared_library::shared_library(const char *name)
        : m_name(name), m_handle(nullptr)
    {
        load();
    }

    shared_library::shared_library(const std::string &name)
        : m_name(name), m_handle(nullptr)
    {
        load();
    }

    shared_library::shared_library(const std::string_view name)
        : m_name(name), m_handle(nullptr)
    {
        load();
    }

    void shared_library::load()
    {
        HMODULE handle = LoadLibrary(m_name.c_str());
        if (!handle) {
            MGE_THROW(system_error) << "Cannot load library '" << m_name << "'";
        }
        m_handle = reinterpret_cast<void *>(handle);
    }

    void *shared_library::symbol(const char *name) const { return nullptr; }

} // namespace mge