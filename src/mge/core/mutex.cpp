#include "mge/core/mutex.hpp"

namespace mge {

    mutex::mutex(const char* name)
        : m_name(name)
    {}

    mutex::~mutex() {}

    void mutex::lock() { m_mutex.lock(); }

    void mutex::unlock() { m_mutex.unlock(); }

    bool mutex::try_lock() { return m_mutex.try_lock(); }

} // namespace mge