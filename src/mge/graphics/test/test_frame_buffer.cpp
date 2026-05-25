// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mock_frame_buffer.hpp"
#include "mock_render_context.hpp"
#include "mock_render_system.hpp"
#include "mock_texture.hpp"
#include "test/googlemock.hpp"

using namespace testing;

class frame_buffer_test : public Test
{
protected:
    void SetUp() override
    {
        rs  = std::make_shared<MOCK_render_system>();
        ctx = std::make_shared<MOCK_render_context>(*rs);
        fb  = std::make_unique<MOCK_frame_buffer>(*ctx);
        tex = std::make_shared<MOCK_texture>(*ctx, mge::texture_type::TYPE_2D);
    }

    std::shared_ptr<MOCK_render_system>  rs;
    std::shared_ptr<MOCK_render_context> ctx;
    std::unique_ptr<MOCK_frame_buffer>   fb;
    mge::texture_ref                     tex;
};

TEST_F(frame_buffer_test, attach_color_calls_on_attach_color)
{
    EXPECT_CALL(*fb, on_attach_color(tex, 0u)).Times(1);
    fb->attach_color(tex);
}

TEST_F(frame_buffer_test, on_attach_color_called_before_state_update)
{
    EXPECT_CALL(*fb, on_attach_color(tex, 0u))
        .WillOnce(Invoke([&](const mge::texture_ref&, uint32_t) {
            EXPECT_FALSE(fb->color_attachment(0));
        }));
    fb->attach_color(tex);
    EXPECT_EQ(fb->color_attachment(0), tex);
}

TEST_F(frame_buffer_test, attach_color_stores_texture)
{
    EXPECT_CALL(*fb, on_attach_color(_, _));
    fb->attach_color(tex);
    EXPECT_EQ(fb->color_attachment(0), tex);
}

TEST_F(frame_buffer_test, attach_color_at_explicit_slot)
{
    EXPECT_CALL(*fb, on_attach_color(tex, 2u)).Times(1);
    fb->attach_color(tex, 2);
    EXPECT_FALSE(fb->color_attachment(0));
    EXPECT_FALSE(fb->color_attachment(1));
    EXPECT_EQ(fb->color_attachment(2), tex);
}

TEST_F(frame_buffer_test, attach_color_replaces_existing_slot)
{
    mge::texture_ref tex2 =
        std::make_shared<MOCK_texture>(*ctx, mge::texture_type::TYPE_2D);
    EXPECT_CALL(*fb, on_attach_color(_, 0u)).Times(2);
    fb->attach_color(tex, 0);
    fb->attach_color(tex2, 0);
    EXPECT_EQ(fb->color_attachment(0), tex2);
}

TEST_F(frame_buffer_test, attach_depth_calls_on_attach_depth)
{
    EXPECT_CALL(*fb, on_attach_depth(tex)).Times(1);
    fb->attach_depth(tex);
}

TEST_F(frame_buffer_test, on_attach_depth_called_before_state_update)
{
    EXPECT_CALL(*fb, on_attach_depth(tex))
        .WillOnce(Invoke([&](const mge::texture_ref&) {
            EXPECT_FALSE(fb->depth_attachment());
        }));
    fb->attach_depth(tex);
    EXPECT_EQ(fb->depth_attachment(), tex);
}

TEST_F(frame_buffer_test, attach_depth_stores_texture)
{
    EXPECT_CALL(*fb, on_attach_depth(_));
    fb->attach_depth(tex);
    EXPECT_EQ(fb->depth_attachment(), tex);
}

TEST_F(frame_buffer_test, color_attachment_empty_before_attach)
{
    EXPECT_FALSE(fb->color_attachment(0));
}

TEST_F(frame_buffer_test, depth_attachment_empty_before_attach)
{
    EXPECT_FALSE(fb->depth_attachment());
}
