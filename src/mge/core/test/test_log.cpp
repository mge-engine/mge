// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/log.hpp"

namespace mge {
    MGE_DEFINE_LOG(FOO);
}

TEST(log, simple_log)
{
    MGE_ERROR_LOG(FOO) << "My hovercraft is full of eels";
}
