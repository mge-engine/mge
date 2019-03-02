// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googlemock.hpp"
#include "mge/graphics/test/mock_window.hpp"

TEST(window, create)
{
    mge::mock_window w(mge::rectangle(0,0,640,480), mge::window_options::standard_options());
}

TEST(window, default_visibility)
{
    auto w = std::make_shared<mge::mock_window>();
    EXPECT_FALSE(w->visible());
}

TEST(window, show)
{
    auto w = std::make_shared<mge::mock_window>();
    EXPECT_CALL(*w, on_show());
    w->show();
    EXPECT_TRUE(w->visible());
}

TEST(window, hide)
{
    auto w = std::make_shared<mge::mock_window>();
    EXPECT_CALL(*w, on_show());
    EXPECT_CALL(*w, on_hide());
    w->show();
    w->hide();
    EXPECT_FALSE(w->visible());
}
