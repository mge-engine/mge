// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googlemock.hpp"
#include "mge/core/test/mock_async_executor.hpp"
#include "mge/graphics/test/mock_render_context.hpp"
#include "mge/graphics/test/mock_hardware_buffer.hpp"

using namespace testing;

TEST(hardware_buffer, map_unmap)
{
    auto executor = std::make_shared<mge::mock_async_executor>();
    auto context = std::make_shared<mge::mock_render_context>(executor.get());
    mge::mock_hardware_buffer buffer(*context,
                                     mge::buffer_type::INDEX_BUFFER,
                                     mge::buffer_usage::DYNAMIC,
                                     mge::buffer_access::READ_WRITE,
                                     mge::buffer_access::READ_WRITE);
    void *testptr = reinterpret_cast<void *>(0x12341234);
    EXPECT_CALL(buffer, on_map()).Times(1).WillOnce(Return(testptr));
    EXPECT_CALL(buffer, on_unmap()).Times(1);
    EXPECT_EQ(testptr, buffer.map());
    buffer.unmap();
}
