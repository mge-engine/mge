// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/test/mock_render_context.hpp"
#include "mge/graphics/test/mock_render_system.hpp"
#include "mge/ui/ui.hpp"
#include "test/googletest.hpp"


namespace mge {

    class ui_test : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            m_render_system = std::make_shared<MOCK_render_system>();
            m_render_context =
                std::make_shared<MOCK_render_context>(*m_render_system);
        }

        void TearDown() override
        {
            m_render_context.reset();
            m_render_system.reset();
        }

        std::shared_ptr<MOCK_render_system>  m_render_system;
        std::shared_ptr<MOCK_render_context> m_render_context;
    };

    TEST_F(ui_test, creation)
    {
        auto ui = std::make_shared<mge::ui>(*m_render_context);
        EXPECT_NE(ui, nullptr);
    }

    TEST_F(ui_test, frame_lifecycle)
    {
        auto ui = std::make_shared<mge::ui>(*m_render_context);
        EXPECT_NO_THROW(ui->frame());
    }

    TEST_F(ui_test, window_lifecycle)
    {
        auto ui = std::make_shared<mge::ui>(*m_render_context);

        bool window_visible = ui->begin_window("Test Window", 0, 0, 200, 200);
        // Window may or may not be visible initially
        (void)window_visible;

        EXPECT_NO_THROW(ui->end_window());
        ui->frame();
    }

    TEST_F(ui_test, button_widget)
    {
        auto ui = std::make_shared<mge::ui>(*m_render_context);

        if (ui->begin_window("Test Window", 0, 0, 200, 200)) {
            bool clicked = ui->button("Test Button");
            // Button should not be clicked in automated test
            EXPECT_FALSE(clicked);
            ui->end_window();
        }

        ui->frame();
    }

    TEST_F(ui_test, checkbox_widget)
    {
        auto ui = std::make_shared<mge::ui>(*m_render_context);

        if (ui->begin_window("Test Window", 0, 0, 200, 200)) {
            bool checked = false;
            ui->checkbox("Test Checkbox", checked);
            ui->end_window();
        }

        ui->frame();
    }

    TEST_F(ui_test, slider_widget)
    {
        auto ui = std::make_shared<mge::ui>(*m_render_context);

        if (ui->begin_window("Test Window", 0, 0, 200, 200)) {
            float value = 50.0f;
            ui->slider(0.0f, value, 100.0f, 1.0f);
            ui->end_window();
        }

        ui->frame();
    }

} // namespace mge
