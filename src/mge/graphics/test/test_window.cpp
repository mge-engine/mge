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
