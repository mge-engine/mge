// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/simple_trace_formatter.hpp"
#include "mge/core/stream_trace_sink.hpp"
#include "mge/core/trace_topic.hpp"
#include "test/googletest.hpp"

using namespace std::chrono_literals;

namespace mge {
    MGE_USE_IMPORTED_TRACE(CORE);
}

TEST(stream_trace_sink, format)
{
    mge::trace_formatter_ref f =
        std::make_shared<mge::simple_trace_formatter>();

    mge::trace_record r;
    std::string       m("My hoovercraft is full of eels");

    r.level = mge::trace_level::WARNING;
    r.sequence = 3;
    r.thread = 0x1234;
    r.time = mge::clock::time_point(0h);
    r.topic = &(MGE_NS_TRACE_TOPIC(mge, CORE));
    r.message = std::string_view(m.begin(), m.end());

    std::stringstream s;

    {
        mge::stream_trace_sink sink(s, f);
        sink.publish(r);
    }

    EXPECT_EQ(std::string("[   0.000000]          3     1234 W              "
                          "CORE My hoovercraft is full of eels\n"),
              s.str());
}