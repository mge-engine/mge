// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/test/mock_async_executor.hpp"
#include "mge/core/to_void_ptr.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/test/mock_render_system.hpp"
#include "test/googlemock.hpp"

#include "mock_render_context.hpp"
#include <memory>
using namespace testing;

namespace mge {
    class test_render_context : public mock_render_context
    {
    public:
        test_render_context(mge::render_system *system, async_executor *ex)
            : mock_render_context(system, ex)
        {}
    };
} // namespace mge

TEST(render_context, dispatches_await)
{
    mge::mock_render_system  system;
    mge::mock_async_executor executor;
    auto                     context =
        std::make_shared<mge::test_render_context>(&system, &executor);
    EXPECT_CALL(executor, await(_));
    context->await([] {});
}

TEST(render_context, dispatches_array_index_buffer_create)
{
    mge::mock_render_system  system;
    mge::mock_async_executor executor;
    auto                     context =
        std::make_shared<mge::test_render_context>(&system, &executor);
    std::array<int, 3> data = {1, 2, 3};
    EXPECT_CALL(*context,
                create_index_buffer(mge::data_type::INT32, mge::usage::DYNAMIC,
                                    3, mge::to_void_ptr(data.data())));

    context->create_index_buffer(mge::usage::DYNAMIC, data);
}
