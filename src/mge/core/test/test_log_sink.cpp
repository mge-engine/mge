// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "test/googlemock.hpp"
#include "mge/core/log_sink.hpp"

using namespace mge;
using namespace testing;

class MOCK_log_sink
        : public log_sink
{
public:
    MOCK_log_sink(bool synchronized)
        :log_sink(synchronized)
    {}

    MOCK_METHOD1(on_publish, void(const log_record&));
};

TEST(log_sink, publish_synchronized)
{
    MOCK_log_sink sink(true);
    log_record r;

    EXPECT_CALL(sink, on_publish(_));
    sink.publish(r);
}

TEST(log_sink, publish_unsynchronized)
{
    MOCK_log_sink sink(false);
    log_record r;

    EXPECT_CALL(sink, on_publish(_));
    sink.publish(r);
}
