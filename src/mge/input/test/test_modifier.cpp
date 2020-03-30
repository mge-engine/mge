// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/input/modifier.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(modifier, print)
{
    mge::test_stream_output(mge::modifier_value::CONTROL, "CONTROL");
    mge::test_stream_output(mge::modifier_value::SHIFT, "SHIFT");
    mge::test_stream_output(mge::modifier_value::ALT, "ALT");
}

TEST(modifier, instantiate) { mge::modifier m; }