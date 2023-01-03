// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/key.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(key, output) { mge::test_stream_output(mge::key::KP_ENTER, "KP_ENTER"); }
