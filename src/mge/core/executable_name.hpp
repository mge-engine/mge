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
     * Get name of the current executable.
     * @return name of executable program,
     *         without path or system-dependent suffixes.
     */
    MGE_CORE_EXPORT std::string executable_name();

}

