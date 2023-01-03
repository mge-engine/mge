// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace.hpp"
#include "mge/core/trace_record.hpp"
#include "mge/core/trace_topic.hpp"
#include "test/googlemock.hpp"
#include "test/googletest.hpp"

using namespace testing;

namespace mge {
    MGE_USE_IMPORTED_TRACE(MGE);
    MGE_USE_IMPORTED_TRACE(CORE);
    class mock_trace_sink : public mge::trace_sink
    {
    public:
        MOCK_METHOD(void, publish, (const mge::trace_record&));
    };

    TEST(trace, topic_sink_called)
    {
        auto sink_1 = std::make_shared<mock_trace_sink>();
        MGE_NS_TRACE_TOPIC(mge, CORE).enable(mge::trace_level::DEBUG);
        MGE_NS_TRACE_TOPIC(mge, CORE).add_sink(sink_1);
        EXPECT_CALL(*sink_1, publish(_))
            .Times(1)
            .WillOnce([&](const mge::trace_record& r) {
                EXPECT_EQ(mge::trace_level::DEBUG, r.level);
                std::string trace_str(r.message.begin(), r.message.end());
                EXPECT_STREQ("Hello", trace_str.c_str());
            });
        MGE_TRACE(CORE, DEBUG) << "Hello";

        MGE_NS_TRACE_TOPIC(mge, CORE).remove_sink(sink_1);
    }

    TEST(trace, global_sink_called)
    {
        auto sink_1 = std::make_shared<mock_trace_sink>();
        MGE_NS_TRACE_TOPIC(mge, CORE).enable(mge::trace_level::DEBUG);
        MGE_NS_TRACE_TOPIC(mge, MGE).add_sink(sink_1);
        EXPECT_CALL(*sink_1, publish(_))
            .Times(1)
            .WillOnce([&](const mge::trace_record& r) {
                EXPECT_EQ(mge::trace_level::DEBUG, r.level);
                std::string trace_str(r.message.begin(), r.message.end());
                EXPECT_STREQ("Hello", trace_str.c_str());
            });
        MGE_TRACE(CORE, DEBUG) << "Hello";

        MGE_NS_TRACE_TOPIC(mge, MGE).remove_sink(sink_1);
    }

} // namespace mge