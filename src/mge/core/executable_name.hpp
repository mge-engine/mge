// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"

#include <string>

/**
 * @file mge/core/executable_name.hpp
 * @brief Name of current executable.
 */

namespace mge {

    /**
     * @brief Get name of the current executable.
     * @return name of executable program,
     *         without path or system-dependent suffixes.
     */
    MGECORE_EXPORT std::string executable_name();

    /**
     * @brief Get path of the current executable.
     * @return path of executable program.
     */
    MGECORE_EXPORT std::string executable_path();

} // namespace mge
