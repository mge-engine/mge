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

#include "mge/core/enum_set.hpp"

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

    /**
     * @brief Window flag values for begin_window and begin_group.
     *
     * Corresponds to nuklear nk_panel_flags.
     */
    enum class ui_window_flag : uint32_t
    {
        BORDER = 0,           //!< Draw border around the window.
        MOVABLE = 1,          //!< Window can be moved by dragging.
        SCALABLE = 2,         //!< Window can be resized.
        CLOSABLE = 3,         //!< Window has a close button.
        MINIMIZABLE = 4,      //!< Window has a minimize button.
        NO_SCROLLBAR = 5,     //!< Disable scrollbar.
        TITLE = 6,            //!< Show title bar.
        SCROLL_AUTO_HIDE = 7, //!< Auto-hide scrollbar when inactive.
        BACKGROUND = 8,       //!< Render as background window.
        SCALE_LEFT = 9,       //!< Scale from left side.
        NO_INPUT = 10,        //!< Ignore all input.
        MAX = 11              //!< Sentinel value.
    };

    /**
     * @brief Set of window flags.
     */
    using ui_window_flags = enum_set<ui_window_flag, ui_window_flag::MAX>;

    /**
     * @brief Text alignment flag values.
     *
     * Corresponds to nuklear nk_text_align. Combine horizontal
     * and vertical flags to specify alignment.
     */
    enum class ui_alignment_flag : uint32_t
    {
        LEFT = 0,     //!< Align text to left.
        CENTERED = 1, //!< Center text horizontally.
        RIGHT = 2,    //!< Align text to right.
        TOP = 3,      //!< Align text to top.
        MIDDLE = 4,   //!< Center text vertically.
        BOTTOM = 5,   //!< Align text to bottom.
        MAX = 6       //!< Sentinel value.
    };

    /**
     * @brief Set of text alignment flags.
     */
    using ui_alignment = enum_set<ui_alignment_flag, ui_alignment_flag::MAX>;

    /**
     * @brief Edit event flag values.
     *
     * Returned by edit_string to indicate widget state.
     * Corresponds to nuklear nk_edit_events.
     */
    enum class ui_edit_event_flag : uint32_t
    {
        ACTIVE = 0,      //!< Widget is currently being modified.
        INACTIVE = 1,    //!< Widget is not active.
        ACTIVATED = 2,   //!< Widget went from inactive to active.
        DEACTIVATED = 3, //!< Widget went from active to inactive.
        COMMITTED = 4,   //!< Widget received enter and lost focus.
        MAX = 5          //!< Sentinel value.
    };

    /**
     * @brief Set of edit event flags.
     */
    using ui_edit_events =
        enum_set<ui_edit_event_flag, ui_edit_event_flag::MAX>;

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
         * @param flags window flags
         * @return true if window is visible and should be rendered
         */
        bool begin_window(const char*     title,
                          float           x,
                          float           y,
                          float           width,
                          float           height,
                          ui_window_flags flags = ui_window_flags());

        /**
         * @brief End current window
         */
        void end_window();

        // ---- Layout ----

        /**
         * @brief Set dynamic row layout
         *
         * @param height row height
         * @param cols number of columns
         */
        void layout_row_dynamic(float height, int cols);

        /**
         * @brief Set static row layout
         *
         * @param height row height
         * @param item_width width of each item
         * @param cols number of columns
         */
        void layout_row_static(float height, int item_width, int cols);

        /**
         * @brief Begin custom row layout
         *
         * @param height row height
         * @param cols number of columns
         */
        void layout_row_begin(float height, int cols);

        /**
         * @brief Push column width ratio in custom row layout
         *
         * @param value width ratio or pixel width
         */
        void layout_row_push(float value);

        /**
         * @brief End custom row layout
         */
        void layout_row_end();

        // ---- Widgets ----

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
        void label(const char* text, ui_alignment alignment = ui_alignment());

        /**
         * @brief Create a colored label
         *
         * @param text label text
         * @param alignment text alignment
         * @param r red
         * @param g green
         * @param b blue
         * @param a alpha
         */
        void label_colored(const char*  text,
                           ui_alignment alignment,
                           uint8_t      r,
                           uint8_t      g,
                           uint8_t      b,
                           uint8_t      a = 255);

        /**
         * @brief Create a word-wrapping label
         *
         * @param text label text
         */
        void label_wrap(const char* text);

        /**
         * @brief Create a checkbox
         *
         * @param label checkbox label
         * @param active checkbox state
         * @return true if state changed
         */
        bool checkbox(const char* label, bool& active);

        /**
         * @brief Create a radio button (option)
         *
         * Only one in a group should be active.
         *
         * @param label option label
         * @param active whether this option is currently selected
         * @return true if this option is now selected
         */
        bool option(const char* label, bool active);

        /**
         * @brief Create a selectable label
         *
         * @param label selectable text
         * @param alignment text alignment
         * @param selected selection state (in/out)
         * @return true if selection state changed
         */
        bool
        selectable(const char* label, ui_alignment alignment, bool& selected);

        /**
         * @brief Create a float slider
         *
         * @param min minimum value
         * @param value current value
         * @param max maximum value
         * @param step step size
         * @return true if value changed
         */
        bool slider(float min, float& value, float max, float step);

        /**
         * @brief Create an integer slider
         *
         * @param min minimum value
         * @param value current value
         * @param max maximum value
         * @param step step size
         * @return true if value changed
         */
        bool slider_int(int min, int& value, int max, int step);

        /**
         * @brief Create a progress bar
         *
         * @param current current value (in/out)
         * @param max maximum value
         * @param modifiable whether user can modify
         * @return true if value changed
         */
        bool progress(size_t& current, size_t max, bool modifiable);

        /**
         * @brief Create an integer property widget
         *
         * @param name property name (displayed as label)
         * @param min minimum value
         * @param val current value (in/out)
         * @param max maximum value
         * @param step step per increment
         * @param inc_per_pixel increment per pixel when dragging
         */
        void property_int(const char* name,
                          int         min,
                          int&        val,
                          int         max,
                          int         step,
                          float       inc_per_pixel);

        /**
         * @brief Create a float property widget
         *
         * @param name property name (displayed as label)
         * @param min minimum value
         * @param val current value (in/out)
         * @param max maximum value
         * @param step step per increment
         * @param inc_per_pixel increment per pixel when dragging
         */
        void property_float(const char* name,
                            float       min,
                            float&      val,
                            float       max,
                            float       step,
                            float       inc_per_pixel);

        /**
         * @brief Create an editable text field
         *
         * @param buffer text buffer
         * @param length current text length (in/out)
         * @param max_length maximum text length
         * @return edit event flags indicating widget state
         */
        ui_edit_events edit_string(char* buffer, int* length, int max_length);

        /**
         * @brief Create a combo box (dropdown)
         *
         * @param items array of item labels
         * @param count number of items
         * @param selected currently selected index
         * @param item_height height of each item
         * @param width popup width
         * @param height popup height
         * @return selected index
         */
        int combo(const char* const* items,
                  int                count,
                  int                selected,
                  int                item_height,
                  float              width,
                  float              height);

        // ---- Spacing / Separators ----

        /**
         * @brief Add empty spacing columns
         *
         * @param cols number of empty columns
         */
        void spacing(int cols);

        /**
         * @brief Add horizontal rule separator
         *
         * @param r red
         * @param g green
         * @param b blue
         * @param a alpha
         */
        void separator(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

        // ---- Groups ----

        /**
         * @brief Begin a scrollable group
         *
         * @param title group title
         * @param flags group flags
         * @return true if group content should be rendered
         */
        bool begin_group(const char* title, ui_window_flags flags);

        /**
         * @brief End current group
         */
        void end_group();

        // ---- Tree ----

        /**
         * @brief Push a tree node
         *
         * @param title node title
         * @param initially_open initial collapse state
         * @return true if node is expanded
         */
        bool tree_push(const char* title, bool initially_open = true);

        /**
         * @brief Pop current tree node
         */
        void tree_pop();

        // ---- Popup ----

        /**
         * @brief Begin a popup window
         *
         * @param type 0 = static, 1 = dynamic
         * @param title popup title
         * @param flags popup flags
         * @param x x position
         * @param y y position
         * @param width popup width
         * @param height popup height
         * @return true if popup is visible
         */
        bool begin_popup(int         type,
                         const char* title,
                         uint32_t    flags,
                         float       x,
                         float       y,
                         float       width,
                         float       height);

        /**
         * @brief Close current popup
         */
        void close_popup();

        /**
         * @brief End current popup
         */
        void end_popup();

        // ---- Tooltip ----

        /**
         * @brief Show simple tooltip text
         *
         * @param text tooltip text
         */
        void tooltip(const char* text);

        // ---- Menubar ----

        /**
         * @brief Begin menubar
         */
        void menubar_begin();

        /**
         * @brief End menubar
         */
        void menubar_end();

        /**
         * @brief Begin a menu
         *
         * @param label menu title
         * @param alignment text alignment
         * @param width menu popup width
         * @param height menu popup height
         * @return true if menu is open
         */
        bool menu_begin(const char*  label,
                        ui_alignment alignment,
                        float        width,
                        float        height);

        /**
         * @brief Create a menu item
         *
         * @param label item label
         * @param alignment text alignment
         * @return true if item was clicked
         */
        bool menu_item(const char*  label,
                       ui_alignment alignment = ui_alignment());

        /**
         * @brief End current menu
         */
        void menu_end();

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
