#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"

#include <string>

namespace moge {

    /**
     * Get name of the current executable.
     * @return name of executable program,
     *         without path or system-dependent suffixes.
     */
    MGE_CORE_EXPORT std::string executable_name();

}

