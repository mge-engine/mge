// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/log.hpp"
#include "mge/core/log_function_guard.hpp"
#include "test/googletest.hpp"

MGE_DEFINE_LOG(FOO);

TEST(log, simple_log) { MGE_ERROR_LOG(FOO) << "My hovercraft is full of eels"; }

TEST(log, function_guard) { MGE_LOG_FUNCTION(FOO); }