// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/test/mock_async_executor.hpp"
#include "mge/graphics/test/mock_index_buffer.hpp"
#include "mge/graphics/test/mock_render_context.hpp"
using namespace testing;
namespace mge {
    TEST(index_buffer, create)
    {
        auto executor = std::make_shared<mock_async_executor>();
        auto context  = std::make_shared<mock_render_context>(executor.get());
        mock_index_buffer buffer(*context, mge::data_type::INT32,
                                 mge::usage::DEFAULT, 17 * sizeof(int),
                                 nullptr);
    }

    TEST(index_buffer, element_count)
    {
        auto executor = std::make_shared<mock_async_executor>();
        auto context  = std::make_shared<mock_render_context>(executor.get());
        mock_index_buffer buffer(*context, mge::data_type::INT32,
                                 mge::usage::DEFAULT, 17 * sizeof(int),
                                 nullptr);
        EXPECT_CALL(buffer, size()).Times(1).WillOnce(Return(17 * sizeof(int)));
        EXPECT_EQ(17u, buffer.element_count());
    }

} // namespace mge