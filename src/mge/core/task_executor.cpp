// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/task_executor.hpp"

namespace mge {
    task_executor::task_executor() {}

    task_executor::~task_executor() {}

    class synchronous_executor : public task_executor
    {
    public:
        void execute(const task_ref &task) override { task(); }
    };

    task_executor_ref task_executor::synchronous_executor()
    {
        static task_executor_ref s_sync_executor =
            std::make_shared<synchronous_executor>();
        return s_sync_executor;
    }

} // namespace mge