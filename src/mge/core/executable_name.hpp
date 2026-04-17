// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"

#include <memory_resource>
#include <string>

/**
 * @file mge/core/executable_name.hpp
 * @brief Name of current executable.
 */

namespace mge {

    /**
     * @brief Get name of the current executable.
     * @param resource memory resource to use for the returned string
     * @return name of executable program,
     *         without path or system-dependent suffixes.
     */
    MGECORE_EXPORT std::pmr::string executable_name(
        std::pmr::memory_resource* resource =
            std::pmr::get_default_resource());

    /**
     * @brief Get path of the current executable.
     * @param resource memory resource to use for the returned string
     * @return path of executable program.
     */
    MGECORE_EXPORT std::pmr::string executable_path(
        std::pmr::memory_resource* resource =
            std::pmr::get_default_resource());

} // namespace mge
