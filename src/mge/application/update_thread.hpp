// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/application/dllexport.hpp"
#include "mge/core/thread.hpp"
#include "mge/core/task_queue.hpp"
#include "mge/core/singleton.hpp"
namespace mge {
    MGE_DECLARE_REF(update_thread);

    class MGE_APPLICATION_EXPORT update_thread
            : public mge::thread
    {
    public:
        update_thread();
        virtual ~update_thread();
        void run() override;
        static update_thread* instance();
    private:
        void execute_work_item();
        task_queue m_tasks;
        static volatile update_thread *s_update_thread;
    };
}
