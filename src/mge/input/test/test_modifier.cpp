// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/modifier.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(modifier, output)
{
    mge::test_stream_output(mge::modifier_value::ALT, "ALT");
}
