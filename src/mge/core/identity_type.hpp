// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#pragma once

namespace mge {

    /**
     * @brief Identity type.
     *
     * This type is used to define a type that is the same as the template
     * argument.
     */
    template <typename T> using identity_type = T;

} // namespace mge