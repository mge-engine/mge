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
    };
} // namespace mge::python