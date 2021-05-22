// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/core/task.hpp"
#include "mge/core/task_queue.hpp"

namespace mge {

    MGE_DECLARE_REF(task_executor);

    /**
     * @brief Mechanism to execute a task.
     */
    class MGECORE_EXPORT task_executor
    {
    public:
        task_executor();
        virtual ~task_executor();

        /**
         * @brief Execute a task and wait for the result.
         *
         * @param task task to execute
         */
        virtual void execute(const task_ref &task) = 0;
    };

} // namespace mge