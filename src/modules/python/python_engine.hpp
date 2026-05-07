// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/script_engine.hpp"

namespace mge::python {

    class python_engine : public mge::script_engine
    {
    public:
        python_engine();
        ~python_engine() override;

        script_context_ref create_context() override;

        PyThreadState* main_thread_state() const noexcept
        {
            return m_main_thread_state;
        }

    private:
        void init_main_state();

        PyConfig       m_config;
        PyThreadState* m_main_thread_state{nullptr};
    };

} // namespace mge::python
