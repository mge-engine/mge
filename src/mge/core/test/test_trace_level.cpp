// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace_level.hpp"
#include "test/googletest.hpp"
#include "test/streamtest.hpp"

TEST(trace_level, stream_output)
{
    EXPECT_STREAMOUTPUT("DEBUG", mge::trace_level::DEBUG);
    EXPECT_STREAMOUTPUT("INFO", mge::trace_level::INFO);
    EXPECT_STREAMOUTPUT("WARNING", mge::trace_level::WARNING);
    EXPECT_STREAMOUTPUT("ERROR", mge::trace_level::ERROR);
    EXPECT_STREAMOUTPUT("FATAL", mge::trace_level::FATAL);
}