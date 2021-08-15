// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <memory>

namespace mge {

    /**
     * @brief Array size of array type.
     *
     * @tparam T        element type of array
     * @tparam size     size of array
     * @return size of array as @c constexpr
     */
    template <typename T, size_t size> constexpr size_t array_size(T (&)[size])
    {
        return size;
    }

} // namespace mge