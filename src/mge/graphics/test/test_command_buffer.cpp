// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/command_buffer.hpp"
#include "mock_render_context.hpp"
#include "test/googletest.hpp"

TEST(command_buffer, initially_empty)
{
    mge::command_buffer cb;
    EXPECT_TRUE(cb.empty());
}

TEST(command_buffer, draw_adds_command)
{
    mge::command_buffer       cb;
    mge::program_handle       prog;
    mge::vertex_buffer_handle vb;
    mge::index_buffer_handle  ib;

    cb.draw(prog, vb, ib);
    EXPECT_FALSE(cb.empty());
}

TEST(command_buffer, clear_empties_buffer)
{
    mge::command_buffer       cb;
    mge::program_handle       prog;
    mge::vertex_buffer_handle vb;
    mge::index_buffer_handle  ib;

    cb.draw(prog, vb, ib);
    cb.clear();
    EXPECT_TRUE(cb.empty());
}

TEST(command_buffer, for_each_verifies_single_draw)
{
    mge::command_buffer       cb;
    mge::program_handle       prog;
    mge::vertex_buffer_handle vb;
    mge::index_buffer_handle  ib;

    cb.draw(prog, vb, ib);

    size_t count = 0;
    cb.for_each([&](const mge::program_handle&       p,
                    const mge::vertex_buffer_handle& v,
                    const mge::index_buffer_handle&  i,
                    const mge::pipeline_state&       state) {
        EXPECT_EQ(p, prog);
        EXPECT_EQ(v, vb);
        EXPECT_EQ(i, ib);
        EXPECT_EQ(state.color_blend_operation(), mge::blend_operation::NONE);
        EXPECT_EQ(state.color_blend_factor_src(), mge::blend_factor::ONE);
        EXPECT_EQ(state.color_blend_factor_dst(), mge::blend_factor::ZERO);
        ++count;
    });
    EXPECT_EQ(count, 1);
}

TEST(command_buffer, for_each_verifies_multiple_draws)
{
    mge::command_buffer       cb;
    mge::program_handle       prog1, prog2, prog3;
    mge::vertex_buffer_handle vb1, vb2, vb3;
    mge::index_buffer_handle  ib1, ib2, ib3;

    cb.draw(prog1, vb1, ib1);
    cb.draw(prog2, vb2, ib2);
    cb.draw(prog3, vb3, ib3);

    size_t count = 0;
    cb.for_each([&](const mge::program_handle&       p,
                    const mge::vertex_buffer_handle& v,
                    const mge::index_buffer_handle&  i,
                    const mge::pipeline_state&       state) {
        ++count;
        if (count == 1) {
            EXPECT_EQ(p, prog1);
            EXPECT_EQ(v, vb1);
            EXPECT_EQ(i, ib1);
        } else if (count == 2) {
            EXPECT_EQ(p, prog2);
            EXPECT_EQ(v, vb2);
            EXPECT_EQ(i, ib2);
        } else if (count == 3) {
            EXPECT_EQ(p, prog3);
            EXPECT_EQ(v, vb3);
            EXPECT_EQ(i, ib3);
        }
    });
    EXPECT_EQ(count, 3);
}

TEST(command_buffer, for_each_verifies_blend_states)
{
    mge::command_buffer       cb;
    mge::program_handle       prog1, prog2, prog3;
    mge::vertex_buffer_handle vb1, vb2, vb3;
    mge::index_buffer_handle  ib1, ib2, ib3;

    cb.blend_opaque();
    cb.draw(prog1, vb1, ib1);

    cb.blend_function(mge::blend_factor::SRC_ALPHA,
                      mge::blend_factor::ONE_MINUS_SRC_ALPHA);
    cb.draw(prog2, vb2, ib2);

    cb.blend_equation(mge::blend_operation::ADD);
    cb.draw(prog3, vb3, ib3);

    size_t count = 0;
    cb.for_each([&](const mge::program_handle&       p,
                    const mge::vertex_buffer_handle& v,
                    const mge::index_buffer_handle&  i,
                    const mge::pipeline_state&       state) {
        ++count;
        if (count == 1) {
            EXPECT_EQ(state.color_blend_operation(),
                      mge::blend_operation::NONE);
            EXPECT_EQ(state.color_blend_factor_src(), mge::blend_factor::ONE);
            EXPECT_EQ(state.color_blend_factor_dst(), mge::blend_factor::ZERO);
        } else if (count == 2) {
            EXPECT_EQ(state.color_blend_operation(),
                      mge::blend_operation::NONE);
            EXPECT_EQ(state.color_blend_factor_src(),
                      mge::blend_factor::SRC_ALPHA);
            EXPECT_EQ(state.color_blend_factor_dst(),
                      mge::blend_factor::ONE_MINUS_SRC_ALPHA);
        } else if (count == 3) {
            EXPECT_EQ(state.color_blend_operation(), mge::blend_operation::ADD);
            EXPECT_EQ(state.color_blend_factor_src(),
                      mge::blend_factor::SRC_ALPHA);
            EXPECT_EQ(state.color_blend_factor_dst(),
                      mge::blend_factor::ONE_MINUS_SRC_ALPHA);
        }
    });
    EXPECT_EQ(count, 3);
}

TEST(command_buffer, depth_write_state)
{
    mge::command_buffer       cb;
    mge::program_handle       prog1, prog2, prog3;
    mge::vertex_buffer_handle vb;
    mge::index_buffer_handle  ib;

    cb.draw(prog1, vb, ib);

    cb.depth_write(true);
    cb.draw(prog2, vb, ib);

    cb.depth_write(false);
    cb.draw(prog3, vb, ib);

    size_t count = 0;
    cb.for_each([&](const mge::program_handle&       p,
                    const mge::vertex_buffer_handle& v,
                    const mge::index_buffer_handle&  i,
                    const mge::pipeline_state&       state) {
        ++count;
        if (count == 1) {
            EXPECT_TRUE(state.depth_write());
        } else if (count == 2) {
            EXPECT_TRUE(state.depth_write());
        } else if (count == 3) {
            EXPECT_FALSE(state.depth_write());
        }
    });
    EXPECT_EQ(count, 3);
}

TEST(command_buffer, depth_test_function_state)
{
    mge::command_buffer       cb;
    mge::program_handle       prog1, prog2, prog3, prog4;
    mge::vertex_buffer_handle vb;
    mge::index_buffer_handle  ib;

    cb.draw(prog1, vb, ib);

    cb.depth_test_function(mge::test::GREATER);
    cb.draw(prog2, vb, ib);

    cb.depth_test_function(mge::test::ALWAYS);
    cb.draw(prog3, vb, ib);

    cb.depth_test_function(mge::test::NEVER);
    cb.draw(prog4, vb, ib);

    size_t count = 0;
    cb.for_each([&](const mge::program_handle&       p,
                    const mge::vertex_buffer_handle& v,
                    const mge::index_buffer_handle&  i,
                    const mge::pipeline_state&       state) {
        ++count;
        if (count == 1) {
            EXPECT_EQ(state.depth_test_function(), mge::test::LESS);
        } else if (count == 2) {
            EXPECT_EQ(state.depth_test_function(), mge::test::GREATER);
        } else if (count == 3) {
            EXPECT_EQ(state.depth_test_function(), mge::test::ALWAYS);
        } else if (count == 4) {
            EXPECT_EQ(state.depth_test_function(), mge::test::NEVER);
        }
    });
    EXPECT_EQ(count, 4);
}
