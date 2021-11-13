// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/script_engine.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/bind.hpp"

namespace mge {
    MGE_DEFINE_TRACE(SCRIPT);

    MGE_REGISTER_COMPONENT(script_engine);

    script_engine::script_engine() { mge::script::bind_all(); }

} // namespace mge