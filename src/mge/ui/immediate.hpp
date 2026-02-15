// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/memory.hpp"
#include "mge/input/key.hpp"
#include "mge/input/key_action.hpp"
#include "mge/input/modifier.hpp"
#include "mge/input/mouse_action.hpp"
#include "mge/ui/dllexport.hpp"

#include <cstdint>

struct nk_context;
struct nk_user_font;
struct nk_font_atlas;

namespace mge {
    class input_handler;

    MGE_DECLARE_REF(immediate_ui);

    /**
     * @brief Immediate mode UI using nuklear library
     *
     * Provides an immediate mode GUI interface for rendering UI elements.
     * This is a wrapper around the nuklear library that fits into the
     * MGE framework.
     */
    class MGEUI_EXPORT immediate_ui
    {
    public:
        /**
         * @brief Construct immediate UI context
         */
        immediate_ui();

        /**
         * @brief Destructor
         */
        ~immediate_ui();

        immediate_ui(const immediate_ui&) = delete;
        immediate_ui& operator=(const immediate_ui&) = delete;
        immediate_ui(immediate_ui&&) = delete;
        immediate_ui& operator=(immediate_ui&&) = delete;

        /**
         * @brief Begin new frame
         *
         * Must be called before any UI commands.
         */
        void begin_frame();

        /**
         * @brief End current frame
         *
         * Must be called after all UI commands for the frame.
         */
        void frame();

        /**
         * @brief Attach to input handler
         *
         * Captures all input events from the handler.
         *
         * @param handler input handler to attach to
         */
        void attach(input_handler& handler);

        /**
         * @brief Detach from input handler
         *
         * Stops capturing input events.
         */
        void detach();

        /**
         * @brief Begin a window
         *
         * @param title window title
         * @param x x position
         * @param y y position
         * @param width window width
         * @param height window height
         * @param flags window flags (nuklear flags)
         * @return true if window is visible and should be rendered
         */
        bool begin_window(const char* title,
                          float       x,
                          float       y,
                          float       width,
                          float       height,
                          uint32_t    flags = 0);

        /**
         * @brief End current window
         */
        void end_window();

        /**
         * @brief Create a button
         *
         * @param label button label
         * @return true if button was clicked
         */
        bool button(const char* label);

        /**
         * @brief Create a label
         *
         * @param text label text
         * @param alignment text alignment
         */
        void label(const char* text, uint32_t alignment = 0);

        /**
         * @brief Create a checkbox
         *
         * @param label checkbox label
         * @param active checkbox state
         * @return true if state changed
         */
        bool checkbox(const char* label, bool& active);

        /**
         * @brief Create a slider
         *
         * @param min minimum value
         * @param value current value
         * @param max maximum value
         * @param step step size
         * @return true if value changed
         */
        bool slider(float min, float& value, float max, float step);

    private:
        void start_frame();

        bool handle_key_action(key k, key_action action, const modifier& m);
        bool handle_mouse_action(uint32_t        button,
                                 mouse_action    action,
                                 const modifier& m,
                                 uint32_t        x,
                                 uint32_t        y);
        bool handle_mouse_move(uint32_t x, uint32_t y);
        bool handle_character(uint32_t ch);
        bool handle_mouse_wheel(int32_t x, int32_t y);

        nk_context*    m_context;
        nk_font_atlas* m_font_atlas;
        bool           m_in_frame{false};
        input_handler* m_input_handler{nullptr};
        uint32_t       m_key_action_handler_key{0};
        uint32_t       m_mouse_action_handler_key{0};
        uint32_t       m_mouse_move_handler_key{0};
        uint32_t       m_character_handler_key{0};
        uint32_t       m_mouse_wheel_handler_key{0};
    };

} // namespace mge
