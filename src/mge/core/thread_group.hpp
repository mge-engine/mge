// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
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

        /**
         * @brief Construct unnamed thread group.
         */
        thread_group();
        /**
         * @brief Construct named thread group.
         * @param name thread group name
         */
        thread_group(const std::string& name);
        /**
         * @brief Destructor.
         */
        ~thread_group();

        /**
         * @brief Get total number of threads in group.
         * @return thread count
         */
        size_type size() const;
        /**
         * @brief Get number of active threads in group.
         * @return active thread count
         */
        size_type active() const;

    private:
        std::string m_name;
    };

} // namespace mge
