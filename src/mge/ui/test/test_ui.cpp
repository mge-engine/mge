// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/test/mock_program.hpp"
#include "mge/graphics/test/mock_render_context.hpp"
#include "mge/graphics/test/mock_render_system.hpp"
#include "mge/graphics/test/mock_shader.hpp"
#include "mge/graphics/test/mock_texture.hpp"
#include "mge/ui/ui.hpp"
#include "test/googletest.hpp"

namespace mge {

    class ui_test : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            using ::testing::_;
            using ::testing::Return;

            m_render_system = std::make_shared<MOCK_render_system>();
            m_render_context =
                std::make_shared<MOCK_render_context>(*m_render_system);

            // Create a mock texture that will be used by UI constructor
            m_mock_texture =
                std::make_shared<MOCK_texture>(*m_render_context,
                                               mge::texture_type::TYPE_2D);

            // Return this texture whenever create_texture is called
            EXPECT_CALL(*m_render_context, create_texture(_))
                .WillRepeatedly(Return(m_mock_texture));

            // Allow any texture set_data calls without failure
            EXPECT_CALL(*m_mock_texture, set_data(_, _, _, _))
                .Times(::testing::AtLeast(0));

            // Return mock shaders and program objects
            EXPECT_CALL(*m_render_context, on_create_shader(_))
                .WillRepeatedly([this](mge::shader_type t) {
                    auto s = new MOCK_shader(*m_render_context, t);
                    ::testing::Mock::AllowLeak(s);
                    EXPECT_CALL(*s, on_compile(::testing::_)).Times(1);
                    EXPECT_CALL(*s, on_set_code(::testing::_))
                        .Times(::testing::AnyNumber());
                    return s;
                });
            EXPECT_CALL(*m_render_context, on_create_program())
                .WillRepeatedly([this]() {
                    auto p = new MOCK_program(*m_render_context);
                    ::testing::Mock::AllowLeak(p);
                    EXPECT_CALL(*p, on_set_shader(::testing::_))
                        .Times(::testing::AnyNumber());
                    EXPECT_CALL(*p, on_link()).Times(::testing::AnyNumber());
                    return p;
                });
            EXPECT_CALL(*m_render_context, on_create_vertex_buffer(_, _))
                .WillRepeatedly(Return(nullptr));
            EXPECT_CALL(*m_render_context, on_create_index_buffer(_, _))
                .WillRepeatedly(Return(nullptr));
            EXPECT_CALL(*m_render_context, on_destroy_shader(_))
                .Times(::testing::AnyNumber());
            EXPECT_CALL(*m_render_context, on_destroy_program(_))
                .Times(::testing::AnyNumber());
        }

        void TearDown() override
        {
            m_mock_texture.reset();
            m_render_context.reset();
            m_render_system.reset();
        }

        std::shared_ptr<MOCK_render_system>  m_render_system;
        std::shared_ptr<MOCK_render_context> m_render_context;
        std::shared_ptr<MOCK_texture>        m_mock_texture;
    };

    TEST_F(ui_test, creation)
    {
        auto ui = std::make_shared<mge::ui>(*m_render_context);
        EXPECT_NE(ui, nullptr);
    }

    TEST_F(ui_test, frame_lifecycle)
    {
        auto ui = std::make_shared<mge::ui>(*m_render_context);
        ui->begin_frame();
        EXPECT_NO_THROW(ui->frame());
    }

    TEST_F(ui_test, window_lifecycle)
    {
        auto ui = std::make_shared<mge::ui>(*m_render_context);
        ui->begin_frame();

        bool window_visible = ui->begin_window("Test Window", 0, 0, 200, 200);
        // Window may or may not be visible initially
        (void)window_visible;

        EXPECT_NO_THROW(ui->end_window());
        ui->frame();
    }

    TEST_F(ui_test, button_widget)
    {
        auto ui = std::make_shared<mge::ui>(*m_render_context);
        ui->begin_frame();

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
        ui->begin_frame();

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
        ui->begin_frame();

        if (ui->begin_window("Test Window", 0, 0, 200, 200)) {
            float value = 50.0f;
            ui->slider(0.0f, value, 100.0f, 1.0f);
            ui->end_window();
        }

        ui->frame();
    }

} // namespace mge
