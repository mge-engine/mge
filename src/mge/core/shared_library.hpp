// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/noncopyable.hpp"
#include <filesystem>
#include <string>
#include <string_view>

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif

namespace mge {

    MGE_DECLARE_REF(shared_library);

    /**
     * @brief A loaded shared library.
     */
    class MGECORE_EXPORT shared_library : noncopyable
    {
    public:
#ifdef MGE_OS_WINDOWS
        using handle_type = HMODULE;

        static constexpr handle_type nil_handle = 0;
#else
#    error Missing port
#endif
        struct use_search_path
        {};

        /**
         * @brief Load shared library, using system search path.
         *
         * @param name library file name
         */
        shared_library(const std::filesystem::path& name,
                       shared_library::use_search_path);

        /**
         * @brief Load shared library.
         *
         * @param name library file name
         */
        shared_library(const std::filesystem::path& name);

        /**
         * @brief File name.
         *
         * @return file name of loaded library
         */
        const std::filesystem::path& name() const { return m_name; }

        /**
         * @brief Resolve a symbol in a shared library.
         *
         * @param name symbol name
         * @return resolved symbol or null pointer if nothing found
         */
        void* symbol(const char* name) const;

        /**
         * @brief Resolve a symbol in a shared library.
         *
         * @param name symbol name
         * @return resolved symbol or null pointer if nothing found
         */
        void* symbol(const std::string& name) const;

    private:
        void load();

        std::filesystem::path m_name;
        handle_type           m_handle;
    };
} // namespace mge