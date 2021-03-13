// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/crash.hpp"
#include "test/googletest.hpp"

TEST(crash, crashes) { EXPECT_DEATH(mge::crash(), "Crash"); }
