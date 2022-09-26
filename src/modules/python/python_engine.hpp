// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/script_engine.hpp"
#include "python.hpp"
#include "python_fwd.hpp"

namespace mge::python {

    class python_engine : public script_engine
    {
    public:
        python_engine();
        ~python_engine() override;

        mge::script_context_ref create_context() override;

        static void interpreter_lost();

        static std::mutex& engine_lock();

    private:
        static void  function_with_address_in_this_module();
        std::wstring compute_python_home();
        void         signal_interpreter_lost();

        static std::mutex                    s_lock;
        static uint64_t                      s_initialized_engines;
        static std::vector<python_engine*>   s_engines;
        static std::wstring                  s_python_home;
        std::vector<python_context_weak_ref> m_contexts;
    };

} // namespace mge::python