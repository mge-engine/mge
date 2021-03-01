// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/noncopyable.hpp"
#include <string>
#include <string_view>

namespace mge {

    class MGECORE_EXPORT shared_library : noncopyable
    {
    public:
        shared_library(const char *name);
        shared_library(std::string_view name);
        shared_library(const std::string &name);

        void *symbol(const char *name) const;
        void *symbol(const std::string &name) const;
        void *symbol(std::string_view name) const;

    private:
        void load();

        std::string m_name;
        void *      m_handle;
    };
} // namespace mge