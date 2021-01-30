// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/trace_level.hpp"
#include "mge/core/trace_topic.hpp"

namespace mge {

    class MGECORE_EXPORT trace
    {};

#define MGE_TRACE(TOPIC, LEVEL) trace(TOPIC, )

} // namespace mge