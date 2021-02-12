// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/exception.hpp"
#include "test/googletest.hpp"

TEST(exception, rethrow) { EXPECT_DEATH(mge::rethrow(), ""); }
