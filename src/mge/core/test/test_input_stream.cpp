// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mock_input_stream.hpp"
#include "test/googletest.hpp"

using namespace testing;

TEST(input_stream, read)
{
    MOCK_input_stream s;
    EXPECT_CALL(s, on_read(NotNull(), 1)).WillOnce(Invoke([](void* dest, auto) {
        *static_cast<char*>(dest) = 'A';
        return 1;
    }));
    EXPECT_EQ('A', s.inherit_read());
}

TEST(input_stream, read_at_eof)
{
    MOCK_input_stream s;
    EXPECT_CALL(s, on_read(NotNull(), 1)).WillOnce(Return(-1));
    EXPECT_EQ(-1, s.inherit_read());
}

TEST(input_stream, read_buffer)
{
    MOCK_input_stream s;
    char              buffer[4] = {0};
    EXPECT_CALL(s, on_read(NotNull(), 3))
        .WillOnce(Invoke([&](void* dest, auto) {
            memcpy(dest, "ABC", 3);
            return 3;
        }));
    EXPECT_EQ(3, s.read(buffer, 3));
    EXPECT_STREQ("ABC", buffer);
}

TEST(input_stream, read_buffer_at_eof)
{
    MOCK_input_stream s;
    char              buffer[4] = {0};
    EXPECT_CALL(s, on_read(NotNull(), 3)).WillOnce(Return(-1));
    EXPECT_EQ(-1, s.read(buffer, 3));
    EXPECT_STREQ("", buffer);
}

TEST(input_stream, read_buffer_with_size_zero)
{
    MOCK_input_stream s;
    char              buffer[4] = {0};
    EXPECT_CALL(s, on_read(NotNull(), 0)).WillOnce(Return(0));
    EXPECT_EQ(0, s.read(buffer, 0));
    EXPECT_STREQ("", buffer);
}

TEST(input_stream, read_buffer_with_size_zero_and_eof)
{
    MOCK_input_stream s;
    char              buffer[4] = {0};
    EXPECT_CALL(s, on_read(NotNull(), 0)).WillOnce(Return(-1));
    EXPECT_EQ(-1, s.read(buffer, 0));
    EXPECT_STREQ("", buffer);
}

TEST(input_stream, read_buffer_full)
{
    MOCK_input_stream s;
    mge::buffer       b;
    bool              read_called = false;
    EXPECT_CALL(s, on_read(NotNull(), _))
        .Times(2)
        .WillRepeatedly(Invoke(
            [&](void* dest, auto size) -> mge::input_stream::streamsize_type {
                if (read_called) {
                    return -1;
                } else {
                    read_called = true;
                }
                memset(dest, 'A', size);
                return size;
            }));
    s.read(b);
    EXPECT_EQ(4096, b.size());
}

TEST(input_stream, tell)
{
    MOCK_input_stream s;
    EXPECT_CALL(s, position()).WillOnce(Return(42));
    EXPECT_EQ(42, s.tell());
}

TEST(input_stream, read_listener)
{
    MOCK_input_stream                  s;
    mge::input_stream::streamsize_type read_bytes = 0;

    auto listener_key = s.add_read_listener(
        [&](mge::input_stream::streamsize_type bytes) { read_bytes += bytes; });

    EXPECT_CALL(s, on_read(NotNull(), 1)).WillOnce(Invoke([](void* dest, auto) {
        *static_cast<char*>(dest) = 'A';
        return 1;
    }));
    EXPECT_EQ('A', s.inherit_read());
    s.remove_read_listener(listener_key);
    EXPECT_EQ(1, read_bytes);
}