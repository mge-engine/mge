// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/input/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {

    /**
     * @brief Key action - press, release or repeat.
     */
    enum class MGEINPUT_EXPORT key_action
    {
        PRESS = 0,
        RELEASE = 1,
        REPEAT = 2
    };

} // namespace mge
