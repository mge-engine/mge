// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/script_engine.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/bind.hpp"

namespace mge {
    MGE_DEFINE_TRACE(SCRIPT);

    MGE_REGISTER_COMPONENT(script_engine);

    class binder
    {
    public:
        binder() { mge::script::bind_core(); }
    };

    static singleton<binder> s_binder;

    script_engine::script_engine() { s_binder.ptr(); }

} // namespace mge