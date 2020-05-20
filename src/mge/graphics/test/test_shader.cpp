// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googlemock.hpp"
#include "test/test_exception.hpp"

#include "mge/core/test/mock_async_executor.hpp"
#include "mge/graphics/test/mock_render_context.hpp"
#include "mge/graphics/test/mock_render_system.hpp"
#include "mge/graphics/test/mock_shader.hpp"

using namespace testing;
namespace mge {

    TEST(shader, is_not_defined_after_create)
    {
        mock_render_system system;
        auto               executor = std::make_shared<mock_async_executor>();
        auto               context =
            std::make_shared<mock_render_context>(&system, executor.get());
        mock_shader_program s(*context, shader_type::VERTEX);
        EXPECT_FALSE(s.defined());
    }

    TEST(shader, compile)
    {
        mock_render_system system;
        auto               executor = std::make_shared<mock_async_executor>();
        auto               context =
            std::make_shared<mock_render_context>(&system, executor.get());
        std::string         source("foo");
        mock_shader_program s(*context, shader_type::VERTEX);
        EXPECT_CALL(s, on_compile(source)).Times(1);
        s.compile(source);
        EXPECT_TRUE(s.defined());
    }

    TEST(shader, compile_with_exception_thrown)
    {
        mock_render_system system;
        auto               executor = std::make_shared<mock_async_executor>();
        auto               context =
            std::make_shared<mock_render_context>(&system, executor.get());
        std::string         source("foo");
        mock_shader_program s(*context, shader_type::VERTEX);
        EXPECT_CALL(s, on_compile(source))
            .Times(1)
            .WillOnce(Throw(mge::bad_cast()));
        try {
            s.compile(source);
            FAIL() << "Exception did not pass through";
        } catch (const mge::bad_cast &) {
        }
        EXPECT_FALSE(s.defined());
    }

    TEST(shader, load)
    {
        mock_render_system system;
        auto               executor = std::make_shared<mock_async_executor>();
        auto               context =
            std::make_shared<mock_render_context>(&system, executor.get());
        mge::buffer         buf;
        mock_shader_program s(*context, shader_type::VERTEX);
        EXPECT_CALL(s, on_load(buf)).Times(1);
        s.load(buf);
        EXPECT_TRUE(s.defined());
    }

    TEST(shader, load_with_exception_thrown)
    {
        mock_render_system system;
        auto               executor = std::make_shared<mock_async_executor>();
        auto               context =
            std::make_shared<mock_render_context>(&system, executor.get());
        mge::buffer         buf;
        mock_shader_program s(*context, shader_type::VERTEX);
        EXPECT_CALL(s, on_load(buf)).Times(1).WillOnce(Throw(mge::bad_cast()));
        try {
            s.load(buf);
            FAIL() << "Exception did not pass through";
        } catch (const mge::bad_cast &) {
        }
        EXPECT_FALSE(s.defined());
    }

} // namespace mge
