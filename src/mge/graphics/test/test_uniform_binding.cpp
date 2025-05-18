// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/uniform_binding.hpp"
#include "mock_program.hpp"
#include "mock_render_context.hpp"
#include "test/googlemock.hpp"
#include "test/googletest.hpp"

TEST(uniform_binding, create_from_null_ref)
{
    mge::program_ref p_null_ref;
    try {
        mge::uniform_binding binding(p_null_ref);
        FAIL() << "Expected exception not thrown";
    } catch (const mge::illegal_argument&) {
        // Expected exception
    }
}

TEST(uniform_binding, create_from_program_ref)
{
    auto             context = std::make_shared<MOCK_render_context>();
    mge::program_ref program = std::make_shared<MOCK_program>(*context);
    EXPECT_CALL(*std::static_pointer_cast<MOCK_program>(program), on_link())
        .Times(1);
    program->link();
    mge::uniform_binding binding(program);
}

TEST(uniform_binding, create_from_program_ref_with_no_link)
{
    auto             context = std::make_shared<MOCK_render_context>();
    mge::program_ref program = std::make_shared<MOCK_program>(*context);
    try {
        mge::uniform_binding binding(program);
        FAIL() << "Expected exception not thrown";
    } catch (const mge::illegal_state&) {
        // Expected exception since the program is not linked
    }
}