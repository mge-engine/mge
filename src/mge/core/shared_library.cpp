// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/shared_library.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/system_error.hpp"
#include <mutex>
#include <windows.h>

namespace mge {

    class loaded_libraries_dict
    {
    public:
        loaded_libraries_dict() = default;
        ~loaded_libraries_dict() = default;

        shared_library::handle_type get(const std::filesystem::path& p)
        {
            std::scoped_lock lock(m_lock);

            auto it = m_libraries.find(p);

            if (it == m_libraries.end()) {
                return shared_library::nil_handle;
            } else {
                return it->second;
            }
        }

        shared_library::handle_type try_put(const std::filesystem::path& name,
                                            shared_library::handle_type  handle)
        {
            std::scoped_lock lock(m_lock);

            auto it = m_libraries.find(name);

            if (it == m_libraries.end()) {
                m_libraries.insert(std::make_pair(name, handle));
                return handle;
            } else {
#ifdef MGE_OS_WINDOWS
                FreeLibrary(handle);
#else
#    error Missing Port
#endif
                return it->second;
            }
        }

    private:
        std::map<std::filesystem::path, shared_library::handle_type>
                   m_libraries;
        std::mutex m_lock;
    };

    singleton<loaded_libraries_dict> s_loaded_libraries;

    shared_library::shared_library(const std::filesystem::path& name)
        : m_handle(nil_handle)
    {
        m_name = std::filesystem::canonical(name);
        load();
    }

    shared_library::shared_library(const std::filesystem::path& name,
                                   shared_library::use_search_path)
        : m_name(name)
        , m_handle(nil_handle)
    {
        load();
    }

    void shared_library::load()
    {
        HMODULE handle = s_loaded_libraries->get(m_name);
        if (handle == nil_handle) {
            handle = LoadLibraryW(m_name.c_str());
            if (!handle) {
                MGE_THROW(system_error)
                    << "Cannot load library '" << m_name << "'";
            }
            handle = s_loaded_libraries->try_put(m_name, handle);
        }

        m_handle = handle;
    }

    void* shared_library::symbol(const char* name) const
    {
        auto address = GetProcAddress(m_handle, name);
        return reinterpret_cast<void*>(address);
    }

    void* shared_library::symbol(const std::string& name) const
    {
        auto address = GetProcAddress(m_handle, name.c_str());
        return reinterpret_cast<void*>(address);
    }

} // namespace mge