// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "python.hpp"

namespace mge::python {

    class gil_lock
    {
    public:
        gil_lock()
        {
            m_gil_state = PyGILState_Ensure();
            m_locked = true;
        }

        ~gil_lock() { release(); }

        gil_lock(const gil_lock&) = delete;
        gil_lock& operator=(const gil_lock&) = delete;

        void release()
        {
            if (m_locked) {
                PyGILState_Release(m_gil_state);
                m_locked = false;
            }
        }

    private:
        PyGILState_STATE m_gil_state;
        bool             m_locked{false};
    };
} // namespace mge::python