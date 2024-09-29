// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "python.hpp"

#include <mutex>

namespace mge::python {

    // Lock that uses the Python GIL as locking mechanism, but works
    // with std::lock_guard
    class gil_lock
    {
    public:
        gil_lock() {}
        ~gil_lock() {}

        void lock() { m_gil_state = PyGILState_Ensure(); }
        void unlock() { PyGILState_Release(m_gil_state); }

        static gil_lock& instance();

    private:
        PyGILState_STATE m_gil_state;
    };

    class gil_lock_guard : public std::lock_guard<gil_lock>
    {
    public:
        gil_lock_guard()
            : std::lock_guard<gil_lock>(gil_lock::instance())
        {}
        ~gil_lock_guard() = default;
    };

} // namespace mge::python