// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <nanobench.h>

namespace mge {

#ifdef MGE_COMPILER_MSVC
#    pragma optimize("", off)
    template <typename T> void do_not_optimize_away(T&& data)
    {
        data = data;
    }
#    pragma optimize("", on)
#elif defined(MGE_COMPILER_GCC) || defined(MGE_COMPILER_APPLECLANG)
    template <typename T> void do_not_optimize_away(T&& data)
    {
        asm volatile("" : "+r"(data) : : "memory");
    }
#else
#    error Missing port
#endif
    using benchmark = ankerl::nanobench::Bench;
} // namespace mge