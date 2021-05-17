#include "mge/core/dllexport.hpp"
#include "mge/core/task.hpp"
#include <mutex>
#include <queue>

namespace mge {

    /**
     * Queue of tasks.
     */
    class task_queue
    {
    public:
        task_queue() {}

        task_queue(const task_queue &q)
        {
            std::lock_guard<std::mutex> guard(q.m_lock);
            m_tasks = q.m_tasks;
        }

        task_queue(task_queue &&q)
        {
            std::lock_guard<std::mutex> guard(q.m_lock);
            m_tasks = std::move(q.m_tasks);
        }

        ~task_queue() = default;

        task_queue &operator=(const task_queue &q)
        {
            if (&q != this) {
                std::lock_guard<std::mutex> guard(q.m_lock);
                m_tasks = q.m_tasks;
            }
            return *this;
        }

        bool empty() const { return m_tasks.empty(); }

        std::queue<task_ref>::size_type size() const { return m_tasks.size(); }

        task_ref pop_front()
        {
            std::lock_guard<decltype(m_lock)> guard(m_lock);
            if (!empty()) {
                auto result = m_tasks.front();
                m_tasks.pop();
                return result;
            } else {
                return task_ref();
            }
        }

        void push_back(const task_ref &t)
        {
            std::lock_guard<decltype(m_lock)> guard(m_lock);
            m_tasks.push(t);
        }

        void clear()
        {
            std::lock_guard<decltype(m_lock)> guard(m_lock);
            std::queue<task_ref>              empty_queue;
            m_tasks.swap(empty_queue);
        }

    private:
        mutable std::mutex   m_lock;
        std::queue<task_ref> m_tasks;
    };

} // namespace mge