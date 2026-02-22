// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/input_handler.hpp"
#include "test/googletest.hpp"

class test_input_handler : public mge::input_handler
{
public:
    test_input_handler() = default;

    bool trigger_key_action(mge::key k, mge::key_action action)
    {
        return on_key_action(k, action);
    }

    bool trigger_mouse_action(uint32_t          button,
                              mge::mouse_action action,
                              uint32_t          x,
                              uint32_t          y)
    {
        return on_mouse_action(button, action, x, y);
    }

    bool trigger_mouse_move(uint32_t x, uint32_t y)
    {
        return on_mouse_move(x, y);
    }

    bool trigger_mouse_wheel(int32_t x, int32_t y)
    {
        return on_mouse_wheel(x, y);
    }

    bool trigger_character(uint32_t ch)
    {
        return on_character(ch);
    }
};

TEST(input_handler, key_action_consumed)
{
    test_input_handler handler;
    bool               handler_called = false;

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            handler_called = true;
            EXPECT_EQ(k, mge::key::A);
            EXPECT_EQ(a, mge::key_action::PRESS);
            return true; // consume event
        });

    bool consumed =
        handler.trigger_key_action(mge::key::A, mge::key_action::PRESS);

    EXPECT_TRUE(handler_called);
    EXPECT_TRUE(consumed);
}

TEST(input_handler, key_action_not_consumed)
{
    test_input_handler handler;
    bool               handler_called = false;

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            handler_called = true;
            return false; // don't consume event
        });

    bool consumed =
        handler.trigger_key_action(mge::key::B, mge::key_action::PRESS);

    EXPECT_TRUE(handler_called);
    EXPECT_FALSE(consumed);
}

TEST(input_handler, key_action_no_handler)
{
    test_input_handler handler;

    bool consumed =
        handler.trigger_key_action(mge::key::C, mge::key_action::PRESS);

    EXPECT_FALSE(consumed);
}

TEST(input_handler, mouse_action_consumed)
{
    test_input_handler handler;
    bool               handler_called = false;

    handler.add_mouse_action_handler([&](uint32_t             button,
                                         mge::mouse_action    action,
                                         const mge::modifier& m,
                                         uint32_t             x,
                                         uint32_t             y) {
        handler_called = true;
        EXPECT_EQ(button, 1u);
        EXPECT_EQ(action, mge::mouse_action::PRESS);
        EXPECT_EQ(x, 100u);
        EXPECT_EQ(y, 200u);
        return true; // consume event
    });

    bool consumed =
        handler.trigger_mouse_action(1, mge::mouse_action::PRESS, 100, 200);

    EXPECT_TRUE(handler_called);
    EXPECT_TRUE(consumed);
}

TEST(input_handler, mouse_action_not_consumed)
{
    test_input_handler handler;

    handler.add_mouse_action_handler([&](uint32_t             button,
                                         mge::mouse_action    action,
                                         const mge::modifier& m,
                                         uint32_t             x,
                                         uint32_t             y) {
        return false; // don't consume
    });

    bool consumed =
        handler.trigger_mouse_action(1, mge::mouse_action::PRESS, 100, 200);

    EXPECT_FALSE(consumed);
}

TEST(input_handler, mouse_move_consumed)
{
    test_input_handler handler;
    bool               handler_called = false;

    handler.add_mouse_move_handler([&](uint32_t x, uint32_t y) {
        handler_called = true;
        EXPECT_EQ(x, 50u);
        EXPECT_EQ(y, 75u);
        return true; // consume
    });

    bool consumed = handler.trigger_mouse_move(50, 75);

    EXPECT_TRUE(handler_called);
    EXPECT_TRUE(consumed);
}

TEST(input_handler, mouse_move_not_consumed)
{
    test_input_handler handler;

    handler.add_mouse_move_handler(
        [&](uint32_t x, uint32_t y) { return false; });

    bool consumed = handler.trigger_mouse_move(50, 75);

    EXPECT_FALSE(consumed);
}

TEST(input_handler, mouse_wheel_consumed)
{
    test_input_handler handler;
    bool               handler_called = false;

    handler.add_mouse_wheel_handler([&](int32_t x, int32_t y) {
        handler_called = true;
        EXPECT_EQ(x, 10);
        EXPECT_EQ(y, -5);
        return true; // consume
    });

    bool consumed = handler.trigger_mouse_wheel(10, -5);

    EXPECT_TRUE(handler_called);
    EXPECT_TRUE(consumed);
}

TEST(input_handler, mouse_wheel_not_consumed)
{
    test_input_handler handler;

    handler.add_mouse_wheel_handler(
        [&](int32_t x, int32_t y) { return false; });

    bool consumed = handler.trigger_mouse_wheel(10, -5);

    EXPECT_FALSE(consumed);
}

TEST(input_handler, character_consumed)
{
    test_input_handler handler;
    bool               handler_called = false;

    handler.add_character_handler([&](uint32_t ch) {
        handler_called = true;
        EXPECT_EQ(ch, 'X');
        return true; // consume
    });

    bool consumed = handler.trigger_character('X');

    EXPECT_TRUE(handler_called);
    EXPECT_TRUE(consumed);
}

TEST(input_handler, character_not_consumed)
{
    test_input_handler handler;

    handler.add_character_handler([&](uint32_t ch) { return false; });

    bool consumed = handler.trigger_character('Y');

    EXPECT_FALSE(consumed);
}

TEST(input_handler, state_maintained_on_consumed_key_event)
{
    test_input_handler handler;

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            return true; // consume
        });

    // Key press
    handler.trigger_key_action(mge::key::A, mge::key_action::PRESS);
    EXPECT_TRUE(handler.state().keyboard().pressed(mge::key::A));

    // Key release
    handler.trigger_key_action(mge::key::A, mge::key_action::RELEASE);
    EXPECT_FALSE(handler.state().keyboard().pressed(mge::key::A));
}

TEST(input_handler, state_maintained_on_not_consumed_key_event)
{
    test_input_handler handler;

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            return false; // don't consume
        });

    // Key press
    handler.trigger_key_action(mge::key::B, mge::key_action::PRESS);
    EXPECT_TRUE(handler.state().keyboard().pressed(mge::key::B));

    // Key release
    handler.trigger_key_action(mge::key::B, mge::key_action::RELEASE);
    EXPECT_FALSE(handler.state().keyboard().pressed(mge::key::B));
}

TEST(input_handler, state_maintained_without_handler)
{
    test_input_handler handler;

    // No handler set
    handler.trigger_key_action(mge::key::C, mge::key_action::PRESS);
    EXPECT_TRUE(handler.state().keyboard().pressed(mge::key::C));

    handler.trigger_key_action(mge::key::C, mge::key_action::RELEASE);
    EXPECT_FALSE(handler.state().keyboard().pressed(mge::key::C));
}

TEST(input_handler, mouse_state_maintained_on_consumed_event)
{
    test_input_handler handler;

    handler.add_mouse_action_handler([&](uint32_t             button,
                                         mge::mouse_action    action,
                                         const mge::modifier& m,
                                         uint32_t             x,
                                         uint32_t             y) {
        return true; // consume
    });

    handler.trigger_mouse_action(1, mge::mouse_action::PRESS, 150, 250);
    EXPECT_TRUE(handler.state().mouse().pressed(1));
    EXPECT_EQ(handler.state().mouse().x(), 150u);
    EXPECT_EQ(handler.state().mouse().y(), 250u);
}

TEST(input_handler, mouse_position_maintained_on_move)
{
    test_input_handler handler;

    handler.add_mouse_move_handler(
        [&](uint32_t x, uint32_t y) { return true; });

    handler.trigger_mouse_move(300, 400);
    EXPECT_EQ(handler.state().mouse().x(), 300u);
    EXPECT_EQ(handler.state().mouse().y(), 400u);
}

TEST(input_handler, clear_handler_prevents_consumption)
{
    test_input_handler handler;

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            return true; // consume
        });

    handler.clear_key_action_handler();

    bool consumed =
        handler.trigger_key_action(mge::key::D, mge::key_action::PRESS);
    EXPECT_FALSE(consumed);
}

TEST(input_handler, multiple_handlers_execution_order)
{
    test_input_handler handler;
    std::vector<int>   execution_order;

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            execution_order.push_back(1);
            return false;
        });

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            execution_order.push_back(2);
            return false;
        });

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            execution_order.push_back(3);
            return false;
        });

    handler.trigger_key_action(mge::key::E, mge::key_action::PRESS);

    ASSERT_EQ(execution_order.size(), 3u);
    EXPECT_EQ(execution_order[0], 1);
    EXPECT_EQ(execution_order[1], 2);
    EXPECT_EQ(execution_order[2], 3);
}

TEST(input_handler, early_termination_on_consumption)
{
    test_input_handler handler;
    bool               first_called = false;
    bool               second_called = false;
    bool               third_called = false;

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            first_called = true;
            return false; // don't consume
        });

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            second_called = true;
            return true; // consume - should stop here
        });

    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            third_called = true;
            return false;
        });

    bool consumed =
        handler.trigger_key_action(mge::key::F, mge::key_action::PRESS);

    EXPECT_TRUE(first_called);
    EXPECT_TRUE(second_called);
    EXPECT_FALSE(third_called); // should not be called
    EXPECT_TRUE(consumed);
}

TEST(input_handler, remove_handler)
{
    test_input_handler handler;
    std::vector<int>   execution_order;

    auto key1 = handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            execution_order.push_back(1);
            return false;
        });

    auto key2 = handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            execution_order.push_back(2);
            return false;
        });

    auto key3 = handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            execution_order.push_back(3);
            return false;
        });

    // Remove middle handler
    handler.remove_key_action_handler(key2);

    handler.trigger_key_action(mge::key::G, mge::key_action::PRESS);

    ASSERT_EQ(execution_order.size(), 2u);
    EXPECT_EQ(execution_order[0], 1);
    EXPECT_EQ(execution_order[1], 3);
}

TEST(input_handler, execution_order_after_removal_and_addition)
{
    test_input_handler handler;
    std::vector<int>   execution_order;

    auto keyA = handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            execution_order.push_back(1);
            return false;
        });

    auto keyB = handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            execution_order.push_back(2);
            return false;
        });

    auto keyC = handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            execution_order.push_back(3);
            return false;
        });

    // Remove B
    handler.remove_key_action_handler(keyB);

    // Add D
    handler.add_key_action_handler(
        [&](mge::key k, mge::key_action a, const mge::modifier& m) {
            execution_order.push_back(4);
            return false;
        });

    handler.trigger_key_action(mge::key::H, mge::key_action::PRESS);

    // Should execute in order: A(1), C(3), D(4)
    ASSERT_EQ(execution_order.size(), 3u);
    EXPECT_EQ(execution_order[0], 1);
    EXPECT_EQ(execution_order[1], 3);
    EXPECT_EQ(execution_order[2], 4);
}

TEST(input_handler, multiple_mouse_handlers)
{
    test_input_handler handler;
    std::vector<int>   execution_order;

    handler.add_mouse_action_handler([&](uint32_t             button,
                                         mge::mouse_action    action,
                                         const mge::modifier& m,
                                         uint32_t             x,
                                         uint32_t             y) {
        execution_order.push_back(1);
        return false;
    });

    handler.add_mouse_action_handler([&](uint32_t             button,
                                         mge::mouse_action    action,
                                         const mge::modifier& m,
                                         uint32_t             x,
                                         uint32_t             y) {
        execution_order.push_back(2);
        return false;
    });

    handler.trigger_mouse_action(1, mge::mouse_action::PRESS, 100, 200);

    ASSERT_EQ(execution_order.size(), 2u);
    EXPECT_EQ(execution_order[0], 1);
    EXPECT_EQ(execution_order[1], 2);
}

TEST(input_handler, remove_nonexistent_handler)
{
    test_input_handler handler;

    // Should not crash when removing non-existent handler
    handler.remove_key_action_handler(999);

    bool consumed =
        handler.trigger_key_action(mge::key::I, mge::key_action::PRESS);
    EXPECT_FALSE(consumed);
}
