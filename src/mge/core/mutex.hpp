// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <mutex>
namespace mge {

    /**
     * @brief Mutex like @c std::mutex but with a name.
     *
     */
    class MGECORE_EXPORT mutex
    {
    public:
        /**
         * @brief Construct a new mutex object
         *
         * @param name name of mutex, need to sustain beyond this object's life
         * time
         */
        mutex(const char* name);

        /**
         * @brief Destroy the mutex object
         */
        ~mutex();

        /**
         * @brief Locks the mutex.
         */
        void lock();

        /**
         * @brief Unlocks the mutex.
         */
        void unlock();

        /**
         * Tries to lock the mutex. Returns immediately. On successful lock
         * acquisition returns  @c true, otherwise returns @c false.
         *
         * @return @c true if lock acquired, @c false otherwise
         */
        bool try_lock();

    private:
        std::mutex  m_mutex;
        const char* m_name;
    };

} // namespace mge