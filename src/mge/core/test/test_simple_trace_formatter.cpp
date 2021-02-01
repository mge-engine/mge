// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/simple_trace_formatter.hpp"
#include "mge/core/trace_topic.hpp"
#include "test/googletest.hpp"
#include <chrono>

using namespace std::chrono_literals;
namespace mge {
    MGE_USE_IMPORTED_TRACE(CORE);
}
TEST(simple_trace_formatter, create)
{
    mge::simple_trace_formatter f;

    mge::trace_record r;
    std::string       m("My hoovercraft is full of eels");

    r.level    = mge::trace_level::WARNING;
    r.sequence = 3;
    r.time     = mge::clock::time_point(0h);
    r.topic    = &(MGE_NS_TRACE_TOPIC(mge, CORE));
    r.message  = std::string_view(m.begin(), m.end());

    std::stringstream s;
    f.format(s, r);
    EXPECT_EQ(std::string("[   0.000000]          3 W              CORE My "
                          "hoovercraft is full of eels\n"),
              s.str());

    r.time = mge::clock::time_point(2h);
    s.str("");
    f.format(s, r);
    EXPECT_EQ(std::string("[7200.000000]          3 W              CORE My "
                          "hoovercraft is full of eels\n"),
              s.str());
}
