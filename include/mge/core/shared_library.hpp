// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/noncopyable.hpp"
#include <string>
#include <string_view>

namespace mge {

    MGE_DECLARE_REF(shared_library);

    /**
     * @brief A loaded shared library.
     */
    class MGECORE_EXPORT shared_library : noncopyable
    {
    public:
        /**
         * @brief Load shared library.
         *
         * @param name library file name
         */
        shared_library(const char *name);
        /**
         * @brief Load shared library.
         *
         * @param name library file name
         */
        shared_library(std::string_view name);
        /**
         * @brief Load shared library.
         *
         * @param name library file name
         */
        shared_library(const std::string &name);

        /**
         * @brief File name.
         *
         * @return file name of loaded library
         */
        const std::string &name() const { return m_name; }

        /**
         * @brief Resolve a symbol in a shared library.
         *
         * @param name symbol name
         * @return resolved symbol or null pointer if nothing found
         */
        void *symbol(const char *name) const;

        /**
         * @brief Resolve a symbol in a shared library.
         *
         * @param name symbol name
         * @return resolved symbol or null pointer if nothing found
         */
        void *symbol(const std::string &name) const;

    private:
        void load();

        std::string m_name;
        void *      m_handle;
    };
} // namespace mge