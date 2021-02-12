// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"

#include <exception>
namespace mge {

    class MGECORE_EXPORT exception : virtual public std::exception
    {};

    /**
     * @brief Re-throws the current exception.
     *
     * This function does not return.
     */
    [[noreturn]] inline void rethrow() { throw; }

} // namespace mge