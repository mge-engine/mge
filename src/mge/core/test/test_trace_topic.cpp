// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/trace_topic.hpp"
#include "test/googletest.hpp"
#include <cstdlib>

namespace mge {
    MGE_USE_IMPORTED_TRACE(MGE);
    MGE_USE_IMPORTED_TRACE(CORE);
} // namespace mge

class trace_topic_test : public ::testing::Test
{
public:
    trace_topic_test() = default;
    virtual ~trace_topic_test() = default;

    static void SetUpTestCase() { mge::configuration::load(true); }
    static bool env_modified()
    {
        return ::getenv("MGE_TRACE_ENABLED") || ::getenv("MGE_TRACE_TO_STDOUT");
    }
};

TEST_F(trace_topic_test, mge_topic)
{
    if (env_modified()) {
        return;
    }
    EXPECT_FALSE(mge::__trace_topic_MGE().enabled(mge::trace_level::ALL));
}

TEST_F(trace_topic_test, core_topic)
{
    if (env_modified()) {
        return;
    }
    EXPECT_FALSE(mge::__trace_topic_CORE().enabled(mge::trace_level::ALL));
}

TEST_F(trace_topic_test, core_topic_name)
{
    auto core_name = std::string("CORE");

    EXPECT_EQ(std::string_view(core_name.begin(), core_name.end()),
              MGE_NS_TRACE_TOPIC(mge, CORE).name());
}

TEST_F(trace_topic_test, global)
{
    EXPECT_TRUE(MGE_NS_TRACE_TOPIC(mge, MGE).global());
    EXPECT_FALSE(MGE_NS_TRACE_TOPIC(mge, CORE).global());
}

TEST_F(trace_topic_test, enable_disable)
{
    if (env_modified()) {
        return;
    }
    EXPECT_FALSE(
        MGE_NS_TRACE_TOPIC(mge, CORE).enabled(mge::trace_level::DEBUG));
    MGE_NS_TRACE_TOPIC(mge, CORE).enable(mge::trace_level::DEBUG);
    EXPECT_TRUE(MGE_NS_TRACE_TOPIC(mge, CORE).enabled(mge::trace_level::DEBUG));
    MGE_NS_TRACE_TOPIC(mge, CORE).disable(mge::trace_level::DEBUG);
    EXPECT_FALSE(
        MGE_NS_TRACE_TOPIC(mge, CORE).enabled(mge::trace_level::DEBUG));
}

TEST_F(trace_topic_test, set_level)
{
    if (env_modified()) {
        return;
    }
    EXPECT_FALSE(
        MGE_NS_TRACE_TOPIC(mge, CORE).enabled(mge::trace_level::DEBUG));
    MGE_NS_TRACE_TOPIC(mge, CORE).set_level(mge::trace_level::DEBUG);
    EXPECT_TRUE(MGE_NS_TRACE_TOPIC(mge, CORE).enabled(mge::trace_level::DEBUG));
    MGE_NS_TRACE_TOPIC(mge, CORE).set_level(mge::trace_level::NONE);
}
