// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/memory.hpp"
#include "mge/ui/dllexport.hpp"

struct nk_context;
struct nk_user_font;

namespace mge {

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

        nk_context*   m_context;
        nk_user_font* m_font;
        bool          m_in_frame{false};
    };

} // namespace mge
