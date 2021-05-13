#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/noncopyable.hpp"

#include <memory>
#include <string>

namespace mge {
    /**
     * @brief A thread group.
     *
     * A thread group is a collection of threads. Threads are added to the
     * group by specifying the thread group at thread construction time,
     * a thread cannot change its group after being assigned to one.
     */
    class MGECORE_EXPORT thread_group
        : public noncopyable,
          public std::enable_shared_from_this<thread_group>
    {
    public:
        using size_type = size_t;

        thread_group();
        thread_group(const std::string &name);
        ~thread_group();

        size_type size() const;
        size_type active() const;

    private:
        std::string m_name;
    };

} // namespace mge
