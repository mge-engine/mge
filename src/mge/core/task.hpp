// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 5204)
#    pragma warning(disable : 5220)
#endif
#include <future>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 5220)
#    pragma warning(default : 5204)
#endif

namespace mge {

    /**
     * @brief Simplified packaged task.
     */
    using task = std::packaged_task<void()>;

    /**
     * @brief Reference to task.
     */
    using task_ref = std::shared_ptr<task>;

} // namespace mge