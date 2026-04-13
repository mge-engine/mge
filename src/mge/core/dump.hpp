// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"

#include <memory_resource>
#include <string>
#include <string_view>

namespace mge {

    /**
     * @brief Crash dump.
     *
     * A crash dump provides useful information for debugging crashes.
     */
    class MGECORE_EXPORT dump
    {
    public:
        dump(std::string_view        purpose = "crash",
             std::pmr::memory_resource* resource =
                 std::pmr::get_default_resource());
        ~dump();

        dump(const dump&) = delete;
        dump& operator=(const dump&) = delete;

        /**
         * @brief Install crash dump handler.
         */
        static void install_handler();

        /**
         * @brief Uninstall crash dump handler.
         */
        static void uninstall_handler();

        /**
         * @brief Write the dump to a file.
         */
        void write();

    private:
        std::pmr::string           m_purpose;
        std::pmr::memory_resource* m_resource;
    };

} // namespace mge
