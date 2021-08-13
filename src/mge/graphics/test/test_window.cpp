// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/test/mock_window.hpp"
#include "test/googletest.hpp"

TEST(window, default_invisible)
{
    MOCK_window w;
    EXPECT_FALSE(w.visible());
}

TEST(window, show)
{
    MOCK_window w;
    EXPECT_CALL(w, on_show()).Times(1);
    w.show();
    w.show(); // no-op
    EXPECT_TRUE(w.visible());
}

TEST(window, hide)
{
    MOCK_window w;
    EXPECT_CALL(w, on_show());
    EXPECT_CALL(w, on_hide());
    w.show();
    w.hide();
    EXPECT_FALSE(w.visible());
}

TEST(window, on_close_calls_listeners)
{
    MOCK_window w;
    bool        closing_listener_called = false;
    bool        close_listener_called = false;
    w.set_closing_listener([&] {
        closing_listener_called = true;
        return true;
    });
    w.set_close_listener([&] { close_listener_called = true; });
    EXPECT_TRUE(w.on_close());
    EXPECT_TRUE(closing_listener_called);
    EXPECT_TRUE(close_listener_called);
}

TEST(window, on_close_aborted_if_closing_listener_returns_false)
{
    MOCK_window w;
    bool        closing_listener_called = false;
    bool        close_listener_called = false;
    w.set_closing_listener([&] {
        closing_listener_called = true;
        return false;
    });
    w.set_close_listener([&] { close_listener_called = true; });
    EXPECT_FALSE(w.on_close());
    EXPECT_TRUE(closing_listener_called);
    EXPECT_FALSE(close_listener_called);
}
