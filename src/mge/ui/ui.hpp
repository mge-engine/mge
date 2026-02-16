// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/memory.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/index_buffer_handle.hpp"
#include "mge/graphics/program_handle.hpp"
#include "mge/graphics/vertex_buffer_handle.hpp"
#include "mge/graphics/vertex_layout.hpp"
#include "mge/input/key.hpp"
#include "mge/input/key_action.hpp"
#include "mge/input/modifier.hpp"
#include "mge/input/mouse_action.hpp"
#include "mge/ui/dllexport.hpp"

#include <cstdint>

struct nk_context;
struct nk_user_font;
struct nk_font_atlas;
struct nk_buffer;
struct nk_convert_config;
struct nk_draw_null_texture;

namespace mge {
    class input_handler;
    class pass;
    class uniform_block;

    MGE_DECLARE_REF(ui);

    /**
     * @brief Immediate mode UI using nuklear library
     *
     * Provides an immediate mode GUI interface for rendering UI elements.
     * This is a wrapper around the nuklear library that fits into the
     * MGE framework.
     */
    class MGEUI_EXPORT ui
    {
    public:
        /**
         * @brief Construct immediate UI context
         *
         * @param context render context
         */
        ui(render_context& context);

        /**
         * @brief Destructor
         */
        ~ui();

        ui(const ui&) = delete;
        ui& operator=(const ui&) = delete;
        ui(ui&&) = delete;
        ui& operator=(ui&&) = delete;

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
         * @brief Set dynamic row layout
         *
         * @param height row height
         * @param cols number of columns
         */
        void layout_row_dynamic(float height, int cols);

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

        /**
         * @brief Create an editable text field
         *
         * @param buffer text buffer
         * @param max_length maximum text length
         * @return current text length
         */
        int edit_string(char* buffer, int* length, int max_length);

        /**
         * @brief Draw UI to render pass
         *
         * @param pass render pass to draw to
         */
        void draw(mge::pass& pass);

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

        render_context* m_render_context;
        nk_context*     m_context;
        nk_font_atlas*  m_font_atlas;
        bool            m_in_frame{false};
        input_handler*  m_input_handler{nullptr};
        uint32_t        m_key_action_handler_key{0};
        uint32_t        m_mouse_action_handler_key{0};
        uint32_t        m_mouse_move_handler_key{0};
        uint32_t        m_character_handler_key{0};
        uint32_t        m_mouse_wheel_handler_key{0};

        // Rendering resources
        texture_ref           m_font_texture;
        program_handle        m_ui_program;
        vertex_buffer_handle  m_vertex_buffer;
        index_buffer_handle   m_index_buffer;
        vertex_layout         m_vertex_layout;
        uniform_block*        m_uniform_block{nullptr};
        nk_buffer*            m_commands{nullptr};
        nk_buffer*            m_vertices{nullptr};
        nk_buffer*            m_indices{nullptr};
        nk_convert_config*    m_convert_config{nullptr};
        nk_draw_null_texture* m_null_texture{nullptr};
    };

} // namespace mge
