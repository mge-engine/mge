// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/shared_library.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/log.hpp"
#include <map>
#include <atomic>


#ifdef MGE_OS_WINDOWS
#  include <windows.h>
#endif

MGE_USE_LOG(MGE);

namespace mge {

    static singleton<std::map<std::string, shared_library_ref> > loaded_libraries;
    static std::mutex loaded_libraries_mutex;

    shared_library::shared_library(const std::string& name, void *handle)
    :m_name(name),
     m_handle(handle)
    {}

    shared_library::~shared_library()
    {}

    const std::string& shared_library::extension()
    {
#ifdef MGE_OS_WINDOWS
        static std::string result(".dll");
        return result;
#elif defined(MGE_OS_MACOSX)
        static std::string result(".dylib");
        return result;
#elif defined(MGE_OS_LINUX)
        static std::string result(".so");
        return result;
#endif
    }

    shared_library_ref shared_library::load(const std::string& name)
    {
        class shared_library_instantiate
            : public shared_library
        {
        public:
            inline shared_library_instantiate(const std::string& name, void *handle)
            :shared_library(name, handle)
            {}
        };

#ifdef MGE_OS_WINDOWS
        std::lock_guard<decltype(loaded_libraries_mutex)> lock(loaded_libraries_mutex);

        auto already_loaded = loaded_libraries->find(name);
        if(already_loaded != loaded_libraries->end()) {
            return already_loaded->second;
        }
        HMODULE handle = LoadLibrary(name.c_str());
        if (!handle) {
            MGE_THROW(system_error).set_info(system_error::error());
        }
        std::shared_ptr<shared_library> library_ref
            = std::make_shared<shared_library_instantiate>(name, handle);
        (*loaded_libraries)[name] = library_ref;
        return library_ref;
#else
#  error Unsupported platform!
#endif
    }

    void *
    shared_library::symbol(const char *name)
    {
#ifdef MGE_OS_WINDOWS
        void *result = (void *)GetProcAddress((HMODULE)m_handle, name);
        auto rc = GetLastError();
        if(!result || rc) {
            MGE_WARNING_LOG(MGE) << "Failed to look up symbol " << name
                                 << " in library " << m_name
                                 << ": " << rc << std::endl;
        } else {
            MGE_WARNING_LOG(MGE) << m_name << "(" << name << ") = " << result << std::endl;
        }

        return result;
#else
#  error Unsupported Plattform!
#endif
    }

    void* shared_library::symbol(const std::string& name)
    {
        return symbol(name.c_str());
    }
}
