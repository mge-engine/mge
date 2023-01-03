// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/script_engine.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(SCRIPT);

    MGE_REGISTER_COMPONENT(script_engine);

    script_engine::script_engine() {}

} // namespace mge