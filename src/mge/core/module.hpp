// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/shared_library.hpp"

#include <vector>

namespace mge {

    MGE_DECLARE_REF(module);

    class MGE_CORE_EXPORT module
    {
    public:
        /**
         * Creates and loads the module.
         * @param name module name
         */
        module(const std::string& name);
        ~module();

        static void load_all();
        static std::vector<module_ref> all_modules();
    private:
        std::string        m_name;
        shared_library_ref m_library;
    };
}
