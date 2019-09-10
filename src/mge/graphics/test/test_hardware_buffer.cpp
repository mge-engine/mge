// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googlemock.hpp"
#include "test/test_exception.hpp"

#include "mge/core/test/mock_async_executor.hpp"
#include "mge/core/stdexceptions.hpp"

#include "mge/graphics/test/mock_render_context.hpp"
#include "mge/graphics/test/mock_hardware_buffer.hpp"


using namespace testing;

TEST(hardware_buffer, map_unmap)
{
    auto executor = std::make_shared<mge::mock_async_executor>();
    auto context = std::make_shared<mge::mock_render_context>(executor.get());
    mge::mock_hardware_buffer buffer(*context,
                                     mge::buffer_type::INDEX_BUFFER,
                                     mge::usage::DYNAMIC);
    void *testptr = reinterpret_cast<void *>(0x12341234);
    EXPECT_CALL(buffer, on_map()).Times(1).WillOnce(Return(testptr));
    EXPECT_CALL(buffer, on_unmap()).Times(1);
    EXPECT_EQ(testptr, buffer.map());
    buffer.unmap();
}

TEST(hardware_buffer, mapped)
{
    auto executor = std::make_shared<mge::mock_async_executor>();
    auto context = std::make_shared<mge::mock_render_context>(executor.get());
    mge::mock_hardware_buffer buffer(*context,
                                     mge::buffer_type::INDEX_BUFFER,
                                     mge::usage::DYNAMIC);
    void *testptr = reinterpret_cast<void *>(0x12341234);
    EXPECT_CALL(buffer, on_map()).Times(1).WillOnce(Return(testptr));
    EXPECT_CALL(buffer, on_unmap()).Times(1);
    buffer.map();
    EXPECT_TRUE(buffer.mapped());
    buffer.unmap();
}


TEST(hardware_buffer, unmappable_buffer)
{
    std::vector<uint32_t> data;
    data.resize(100);

    auto executor = std::make_shared<mge::mock_async_executor>();
    auto context = std::make_shared<mge::mock_render_context>(executor.get());
    mge::mock_hardware_buffer unmappable_buffer(*context,
                                     mge::buffer_type::INDEX_BUFFER,
                                     mge::usage::IMMUTABLE,
                                     data.data(),
                                     data.size() * sizeof(uint32_t));
    EXPECT_FALSE(unmappable_buffer.mappable());
}

TEST(hardware_buffer, mappable_buffer)
{
    mge::usage usage[] = { mge::usage::DEFAULT,
                           mge::usage::DYNAMIC,
                           mge::usage::STAGING };
    for(const auto& u : usage) {
        auto executor = std::make_shared<mge::mock_async_executor>();
        auto context = std::make_shared<mge::mock_render_context>(executor.get());
        mge::mock_hardware_buffer buffer(*context,
                                         mge::buffer_type::INDEX_BUFFER,
                                         u);
        EXPECT_TRUE(buffer.mappable());
    }
}

TEST(hardware_buffer, construct_unmapped_unmappable_throws)
{
    auto executor = std::make_shared<mge::mock_async_executor>();
    auto context = std::make_shared<mge::mock_render_context>(executor.get());
    EXPECT_THROW_WITH_MESSAGE(mge::mock_hardware_buffer buffer(*context,
                                                               mge::buffer_type::INDEX_BUFFER,
                                                               mge::usage::IMMUTABLE),
                              mge::illegal_argument,
                              "Unsupported usage for unmapped buffer");
}
