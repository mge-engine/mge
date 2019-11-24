// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "test/googlemock.hpp"
#include "mge/core/output_stream.hpp"

#include <iomanip>

using namespace testing;

class MOCK_output_stream : public mge::output_stream
{
public:
    MOCK_output_stream() = default;
    ~MOCK_output_stream() = default;

    MOCK_METHOD2(on_write, void(const void*, mge::output_stream::streamsize_type));
    MOCK_METHOD0(on_flush, void());
};

TEST(output_stream, simple_write)
{
    MOCK_output_stream s; 
    EXPECT_CALL(s, on_write(_, 1)).Times(1);
    s.write('A');  
}

TEST(output_stream, write_buffer)
{
    const char* buffer = "AAA";
    MOCK_output_stream s;
    EXPECT_CALL(s, on_write(_, 3)).Times(1);
    s.write(buffer, 3);
}


TEST(output_stream, flush)
{
    MOCK_output_stream s;
    EXPECT_CALL(s, on_flush()).Times(1);
    s.flush();
}

TEST(output_stream, write_listener)
{
    MOCK_output_stream s;
    std::streamsize written = 0;
    auto listener_key = s.add_write_listener([&](std::streamsize s) {
        written += s;
                         });

    EXPECT_CALL(s, on_write(_, 1)).Times(1);
    s.write('A');
    s.remove_write_listener(listener_key);
    EXPECT_EQ(1, written);
}

TEST(output_stream, stdstream_write)
{
    MOCK_output_stream s;
    EXPECT_CALL(s, on_write(_, 1)).Times(1);
    EXPECT_CALL(s, on_flush()).Times(1);
    s.ostream() << "A" << std::flush;
}



