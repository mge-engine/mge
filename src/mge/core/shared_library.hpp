/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#pragma once

#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include <string>
#include <memory>

namespace mge {

    MGE_DECLARE_REF(shared_library);

    class MGE_CORE_EXPORT shared_library
    {
    public:
        ~shared_library();
        static shared_library_ref load(const std::string& name);

        void *symbol(const std::string& name);
        void *symbol(const char *name);

        static const std::string& suffix();
    protected:
        shared_library(const std::string& name, void *handle);
    private:
        std::string m_name;
        void       *m_handle;
    };
}
