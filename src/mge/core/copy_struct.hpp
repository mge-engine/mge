// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
#include "mge/config.hpp"

#include <cstring>

namespace mge {

    /**
     * @brief Plain memory copy of a structure into another.
     *
     * @tparam T type copied
     * @param target target value
     * @param source source value
     */
    template <typename T> inline void copy_struct(T& target, const T& source)
    {
        memcpy(&target, &source, sizeof(T));
    }
} // namespace mge