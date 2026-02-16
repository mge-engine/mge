// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/ui/ui.hpp"
#include "test/googletest.hpp"

namespace mge {

    TEST(ui, creation)
    {
        auto ui = std::make_shared<mge::ui>();
        EXPECT_NE(ui, nullptr);
    }

    TEST(ui, frame_lifecycle)
    {
        auto ui = std::make_shared<mge::ui>();
        EXPECT_NO_THROW(ui->frame());
    }

    TEST(ui, window_lifecycle)
    {
        auto ui = std::make_shared<mge::ui>();

        bool window_visible = ui->begin_window("Test Window", 0, 0, 200, 200);
        // Window may or may not be visible initially
        (void)window_visible;

        EXPECT_NO_THROW(ui->end_window());
        ui->frame();
    }

    TEST(ui, button_widget)
    {
        auto ui = std::make_shared<mge::ui>();

        if (ui->begin_window("Test Window", 0, 0, 200, 200)) {
            bool clicked = ui->button("Test Button");
            // Button should not be clicked in automated test
            EXPECT_FALSE(clicked);
            ui->end_window();
        }

        ui->frame();
    }

    TEST(ui, checkbox_widget)
    {
        auto ui = std::make_shared<mge::ui>();

        if (ui->begin_window("Test Window", 0, 0, 200, 200)) {
            bool checked = false;
            ui->checkbox("Test Checkbox", checked);
            ui->end_window();
        }

        ui->frame();
    }

    TEST(ui, slider_widget)
    {
        auto ui = std::make_shared<mge::ui>();

        if (ui->begin_window("Test Window", 0, 0, 200, 200)) {
            float value = 50.0f;
            ui->slider(0.0f, value, 100.0f, 1.0f);
            ui->end_window();
        }

        ui->frame();
    }

} // namespace mge
