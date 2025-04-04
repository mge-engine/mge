// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/script/script_engine.hpp"
#include "python.hpp"
#include "python_fwd.hpp"

#include <atomic>
#include <string>
#include <vector>
namespace mge::python {

    class python_engine : public script_engine
    {
    public:
        python_engine();
        ~python_engine() override;

        mge::script_context_ref create_context() override;

        static void interpreter_lost();

        bool interpreter_initialized() const;
        void initialize_interpreter();

        void on_interpreter_loss();
        void on_interpreter_restore();

    private:
        python_context_ref m_context;
    };
} // namespace mge::python