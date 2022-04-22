// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/crash.hpp"
#include "mge/core/trace_level.hpp"

#include "mge/script/bind.hpp"
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"

namespace mge::script {
    void bind_core()
    {
        module("mge")(function("crash", &mge::crash),
                      type<mge::trace_level>("trace_level")
                          .enum_value("NONE", mge::trace_level::NONE)
                          .enum_value("DEBUG", mge::trace_level::DEBUG)
                          .enum_value("INFO", mge::trace_level::INFO)
                          .enum_value("WARNING", mge::trace_level::WARNING)
                          .enum_value("ERROR", mge::trace_level::LEVEL_ERROR)
                          .enum_value("FATAL", mge::trace_level::FATAL)
                          .enum_value("ALL", mge::trace_level::ALL));
    }
} // namespace mge::script