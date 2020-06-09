// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <cstdlib>
#include <memory>
namespace mge {

    /**
     * @brief Copies a structure.
     *
     * @tparam T structure type, must not contain types needing copy constructor
     * @param target target
     * @param source source
     */
    template <typename T> inline void copy_struct(T &target, const T &source)
    {
        memcpy(std::addressof(target), std::addressof(source), sizeof(T));
    }

    /**
     * @brief Copies a structure.
     *
     * @tparam T structure type, must not contain types needing copy constructor
     * @param target pointer to target
     * @param source source
     */
    template <typename T> inline void copy_struct(T *target, const T &source)
    {
        memcpy(target, std::addressof(source), sizeof(T));
    }

} // namespace mge
