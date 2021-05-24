#pragma once
#include "mge/input/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {

    /**
     * @brief Key action - press, release or repeat.
     */
    enum class MGEINPUT_EXPORT key_action
    {
        PRESS   = 0,
        RELEASE = 1,
        REPEAT  = 2
    };

    MGEINPUT_EXPORT std::ostream &operator<<(std::ostream &    os,
                                             const key_action &a);
} // namespace mge
