// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/test/mock_window.hpp"
#include "test/googlemock.hpp"

TEST(window, create)
{
    mge::mock_window w(mge::rectangle(0, 0, 640, 480),
                       mge::window_options::standard_options());
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
    EXPECT_CALL(*w, install_display_thread());
    w->show();
    EXPECT_TRUE(w->visible());
}

TEST(window, show_shown)
{
    auto w = std::make_shared<testing::StrictMock<mge::mock_window>>();
    EXPECT_CALL(*w, on_show()).Times(1);
    EXPECT_CALL(*w, install_display_thread()).Times(1);
    w->show();
    w->show();
    EXPECT_TRUE(w->visible());
}

TEST(window, hide)
{
    auto w = std::make_shared<mge::mock_window>();
    EXPECT_CALL(*w, on_show());
    EXPECT_CALL(*w, install_display_thread());
    EXPECT_CALL(*w, on_hide());
    w->show();
    w->hide();
    EXPECT_FALSE(w->visible());
}

TEST(window, hide_hidden)
{
    auto w = std::make_shared<testing::StrictMock<mge::mock_window>>();
    w->hide();
    EXPECT_FALSE(w->visible());
}
