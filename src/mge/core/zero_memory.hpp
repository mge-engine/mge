// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
#include "mge/config.hpp"

#include <memory>

namespace mge {

    /**
     * @brief Clears the memory of a structure.
     *
     * @tparam T    type of cleared value
     * @param value cleared value
     */
    template <typename T> inline void zero_memory(T& value)
    {
        auto ptr = std::addressof(value);
        memset(ptr, 0, sizeof(T));
    }
} // namespace mge