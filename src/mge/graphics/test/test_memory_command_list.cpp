// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googlemock.hpp"
#include "test/test_exception.hpp"

#include "mge/core/stdexceptions.hpp"
#include "mge/core/test/mock_async_executor.hpp"

#include "mge/graphics/memory_command_list.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/test/mock_render_context.hpp"
#include "mge/graphics/test/mock_render_system.hpp"

using namespace testing;
namespace mge {
    TEST(memory_command_list, create)
    {
        mock_render_system system;
        auto executor = std::make_shared<mge::mock_async_executor>();
        auto context =
            std::make_shared<mge::mock_render_context>(&system, executor.get());

        mge::memory_command_list l(*context);
        EXPECT_FALSE(l.native());
        EXPECT_FALSE(l.finished());
        EXPECT_FALSE(l.immutable());
    }

    TEST(memory_command_list, clear)
    {
        mock_render_system system;
        auto executor = std::make_shared<mge::mock_async_executor>();
        auto context =
            std::make_shared<mge::mock_render_context>(&system, executor.get());

        mge::memory_command_list l(*context);
        l.clear(mge::rgba_color(0xFFFFFFFF));
        l.finish();
        EXPECT_EQ(1u, std::distance(l.begin(), l.end()));
    }
} // namespace mge