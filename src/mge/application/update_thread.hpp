#pragma once
#include "mge/application/dllexport.hpp"
#include "mge/core/thread.hpp"
#include "mge/core/task_queue.hpp"
#include "mge/core/singleton.hpp"
namespace mge {
    class MGE_APPLICATION_EXPORT update_thread
            : public mge::thread
    {
    public:
        update_thread();
        virtual ~update_thread();
        void run() override;
        static update_thread* instance();
    private:
        task_queue m_tasks;
        static volatile update_thread *s_update_thread;
    };
}
