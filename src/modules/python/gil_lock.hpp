#pragma once
#include "python.hpp"

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

    }; // namespace mge::python

} // namespace mge::python