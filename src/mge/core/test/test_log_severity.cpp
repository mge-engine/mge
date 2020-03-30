// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/log_severity.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(log_severity, stream_output)
{
    mge::test_stream_output(mge::log_severity::NONE, "NONE");
    mge::test_stream_output(mge::log_severity::ERROR_SEVERITY, "ERROR");
    mge::test_stream_output(mge::log_severity::WARNING_SEVERITY, "WARNING");
    mge::test_stream_output(mge::log_severity::INFO_SEVERITY, "INFO");
    mge::test_stream_output(mge::log_severity::DEBUG_SEVERITY, "DEBUG");
    mge::test_stream_output(mge::log_severity::ALL, "ALL");
    mge::test_stream_output((mge::log_severity)(123), "INVALID(123)");
}
