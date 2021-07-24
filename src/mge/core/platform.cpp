// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/trace_topic.hpp"

#ifdef MGE_OS_WINDOWS
namespace mge {
    MGE_DEFINE_TRACE(WIN32);
}
#endif