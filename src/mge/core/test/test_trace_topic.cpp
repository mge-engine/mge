// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace_topic.hpp"
#include "test/googletest.hpp"

namespace mge {
    MGE_USE_IMPORTED_TRACE(MGE);
    MGE_USE_IMPORTED_TRACE(CORE);
} // namespace mge

TEST(trace_topic, mge_topic)
{
    EXPECT_FALSE(mge::__trace_topic_MGE().enabled(mge::trace_level::ALL));
}

TEST(trace_topic, core_topic)
{
    EXPECT_FALSE(mge::__trace_topic_CORE().enabled(mge::trace_level::ALL));
}

TEST(trace_topic, core_topic_name)
{
    auto core_name = std::string("CORE");

    EXPECT_EQ(std::string_view(core_name.begin(), core_name.end()),
              MGE_NS_TRACE_TOPIC(mge, CORE).name());
}

TEST(trace_topic, global)
{
    EXPECT_TRUE(MGE_NS_TRACE_TOPIC(mge, MGE).global());
    EXPECT_FALSE(MGE_NS_TRACE_TOPIC(mge, CORE).global());
}

TEST(trace_topic, enable_disable)
{
    EXPECT_FALSE(
        MGE_NS_TRACE_TOPIC(mge, CORE).enabled(mge::trace_level::DEBUG));
    MGE_NS_TRACE_TOPIC(mge, CORE).enable(mge::trace_level::DEBUG);
    EXPECT_TRUE(MGE_NS_TRACE_TOPIC(mge, CORE).enabled(mge::trace_level::DEBUG));
    MGE_NS_TRACE_TOPIC(mge, CORE).disable(mge::trace_level::DEBUG);
    EXPECT_FALSE(
        MGE_NS_TRACE_TOPIC(mge, CORE).enabled(mge::trace_level::DEBUG));
}

TEST(trace_topic, set_level)
{
    EXPECT_FALSE(
        MGE_NS_TRACE_TOPIC(mge, CORE).enabled(mge::trace_level::DEBUG));
    MGE_NS_TRACE_TOPIC(mge, CORE).set_level(mge::trace_level::DEBUG);
    EXPECT_TRUE(MGE_NS_TRACE_TOPIC(mge, CORE).enabled(mge::trace_level::DEBUG));
    MGE_NS_TRACE_TOPIC(mge, CORE).set_level(mge::trace_level::NONE);
}
