#include "mge/core/task.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    task::task(const std::function<void()> &f) : m_function(f) {}

    task::task(std::function<void()> &&f) : m_function(std::move(f)) {}

    task::~task() {}

    void task::run()
    {
        try {
            if (m_function) {
                m_function();
            }
            m_result.set_value();
        } catch (...) {
            try {
                m_result.set_exception(std::current_exception());
            } catch (...) {
                // todo: crash
            }
        }
    }

    void task::wait() { m_result.get_future().wait(); }

} // namespace mge