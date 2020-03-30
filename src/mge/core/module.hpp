// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/shared_library.hpp"
#include "mge/core/types.hpp"

#include <vector>

namespace mge {

    MGE_DECLARE_REF(module);

    class MGE_CORE_EXPORT module
    {

        /**
         * Creates and loads the module.
         * @param name module name
         */
    protected:
        module(const std::string &name);

    public:
        ~module();

        /**
         * @brief Load all modules.
         *
         * Scans configuration and loads all modules.
         */
        static void load_all();

        /**
         * @brief Retrieve all loaded modules.
         *
         * @return vector of loaded modules
         */
        static std::vector<module_ref> all_modules();

        /**
         * @brief Module name.
         *
         * @return module name
         */
        const std::string &name() const;

        /**
         * @brief Module shared library.
         *
         * @return Shared library implemnting the module.
         */
        const mge::shared_library_ref library() const;

    private:
        std::string        m_name;
        shared_library_ref m_library;
    };
} // namespace mge
