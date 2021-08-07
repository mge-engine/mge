// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mock_hardware_buffer.hpp"
#include "mock_render_context.hpp"
#include "test/googletest.hpp"

using namespace testing;

TEST(hardware_buffer, map_unmap)
{
    auto ctx = std::make_shared<MOCK_render_context>();

    MOCK_hardware_buffer buffer(*ctx, mge::buffer_type::INDEX, 1024, nullptr);
    void*                dataptr = reinterpret_cast<void*>(0x1234567);
    EXPECT_EQ(1024u, buffer.size());
    EXPECT_FALSE(buffer.mapped());
    EXPECT_CALL(buffer, on_map()).Times(1).WillOnce(Return(dataptr));
    void* result = buffer.map();
    EXPECT_TRUE(buffer.mapped());
    EXPECT_EQ(dataptr, result);
    void* result2 = buffer.map(); // no 2nd on_map call
    EXPECT_EQ(result, result2);
    EXPECT_TRUE(buffer.mapped());
    EXPECT_CALL(buffer, on_unmap()).Times(1);
    buffer.unmap();
    EXPECT_TRUE(buffer.mapped());
    buffer.unmap();
    EXPECT_FALSE(buffer.mapped());
}