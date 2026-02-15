// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/ui/immediate.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/input/input_handler.hpp"
#include "mge/input/key.hpp"
#include "mge/input/key_action.hpp"
#include "mge/input/modifier.hpp"
#include "mge/input/mouse_action.hpp"

#define NK_INCLUDE_FIXED_TYPES

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4701) // potentially uninitialized variable
#endif

#include <nuklear.h>

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

namespace mge {

    static void* nk_mge_alloc(nk_handle unused, void* old, nk_size size)
    {
        (void)unused;
        return mge::realloc(old, size);
    }

    static void nk_mge_free(nk_handle unused, void* ptr)
    {
        (void)unused;
        mge::free(ptr);
    }

    class nk_allocator_instance
    {
    public:
        nk_allocator_instance()
        {
            m_allocator.userdata = nk_handle_ptr(nullptr);
            m_allocator.alloc = nk_mge_alloc;
            m_allocator.free = nk_mge_free;
        }

        nk_allocator* get()
        {
            return &m_allocator;
        }

        static mge::singleton<nk_allocator_instance> instance;

    private:
        nk_allocator m_allocator;
    };

    mge::singleton<nk_allocator_instance> nk_allocator_instance::instance;

    static float
    dummy_font_width(nk_handle handle, float h, const char* text, int len)
    {
        (void)handle;
        (void)h;
        (void)text;
        return len * 10.0f; // Simple fixed-width font for testing
    }

    immediate_ui::immediate_ui()
        : m_context(new nk_context())
        , m_font(new nk_user_font())
    {
        m_font->userdata = nk_handle_ptr(nullptr);
        m_font->height = 14.0f;
        m_font->width = dummy_font_width;

        nk_init(m_context, nk_allocator_instance::instance->get(), m_font);
        start_frame();
    }

    immediate_ui::~immediate_ui()
    {
        detach();
        if (m_context) {
            nk_free(m_context);
            delete m_context;
            m_context = nullptr;
        }
        if (m_font) {
            delete m_font;
            m_font = nullptr;
        }
    }

    void immediate_ui::start_frame()
    {
        nk_clear(m_context);
        m_in_frame = true;
    }

    void immediate_ui::frame()
    {
        if (!m_in_frame) {
            MGE_THROW(mge::illegal_state)
                << "Cannot end frame without beginning it first";
        }
        m_in_frame = false;
        start_frame();
    }

    bool immediate_ui::begin_window(const char* title,
                                    float       x,
                                    float       y,
                                    float       width,
                                    float       height,
                                    uint32_t    flags)
    {
        return nk_begin(m_context,
                        title,
                        nk_rect(x, y, width, height),
                        static_cast<nk_flags>(flags)) != 0;
    }

    void immediate_ui::end_window()
    {
        nk_end(m_context);
    }

    bool immediate_ui::button(const char* label)
    {
        return nk_button_label(m_context, label) != 0;
    }

    void immediate_ui::label(const char* text, uint32_t alignment)
    {
        nk_label(m_context, text, static_cast<nk_flags>(alignment));
    }

    bool immediate_ui::checkbox(const char* label, bool& active)
    {
        int state = active ? 1 : 0;
        int result = nk_checkbox_label(m_context, label, &state);
        active = state != 0;
        return result != 0;
    }

    bool immediate_ui::slider(float min, float& value, float max, float step)
    {
        return nk_slider_float(m_context, min, &value, max, step) != 0;
    }

    void immediate_ui::attach(input_handler& handler)
    {
        detach();
        m_input_handler = &handler;
        m_key_action_handler_key = handler.add_key_action_handler(
            [this](key k, key_action action, const modifier& m) -> bool {
                return handle_key_action(k, action, m);
            });
        m_mouse_action_handler_key =
            handler.add_mouse_action_handler([this](uint32_t        button,
                                                    mouse_action    action,
                                                    const modifier& m,
                                                    uint32_t        x,
                                                    uint32_t        y) -> bool {
                return handle_mouse_action(button, action, m, x, y);
            });
        m_mouse_move_handler_key = handler.add_mouse_move_handler(
            [this](uint32_t x, uint32_t y) -> bool {
                return handle_mouse_move(x, y);
            });
        m_character_handler_key = handler.add_character_handler(
            [this](uint32_t ch) -> bool { return handle_character(ch); });
        m_mouse_wheel_handler_key = handler.add_mouse_wheel_handler(
            [this](int32_t x, int32_t y) -> bool {
                return handle_mouse_wheel(x, y);
            });
    }

    void immediate_ui::detach()
    {
        if (m_input_handler) {
            if (m_key_action_handler_key) {
                m_input_handler->remove_key_action_handler(
                    m_key_action_handler_key);
                m_key_action_handler_key = 0;
            }
            if (m_mouse_action_handler_key) {
                m_input_handler->remove_mouse_action_handler(
                    m_mouse_action_handler_key);
                m_mouse_action_handler_key = 0;
            }
            if (m_mouse_move_handler_key) {
                m_input_handler->remove_mouse_move_handler(
                    m_mouse_move_handler_key);
                m_mouse_move_handler_key = 0;
            }
            if (m_character_handler_key) {
                m_input_handler->remove_character_handler(
                    m_character_handler_key);
                m_character_handler_key = 0;
            }
            if (m_mouse_wheel_handler_key) {
                m_input_handler->remove_mouse_wheel_handler(
                    m_mouse_wheel_handler_key);
                m_mouse_wheel_handler_key = 0;
            }
            m_input_handler = nullptr;
        }
    }

    bool
    immediate_ui::handle_key_action(key k, key_action action, const modifier& m)
    {
        nk_input_begin(m_context);

        bool down =
            (action == key_action::PRESS || action == key_action::REPEAT);

        // Map MGE keys to Nuklear keys
        switch (k) {
        case key::BACKSPACE:
            nk_input_key(m_context, NK_KEY_BACKSPACE, down);
            break;
        case key::DELETE_KEY:
            nk_input_key(m_context, NK_KEY_DEL, down);
            break;
        case key::ENTER:
            nk_input_key(m_context, NK_KEY_ENTER, down);
            break;
        case key::TAB:
            nk_input_key(m_context, NK_KEY_TAB, down);
            break;
        case key::CURSOR_UP:
            nk_input_key(m_context, NK_KEY_UP, down);
            break;
        case key::CURSOR_DOWN:
            nk_input_key(m_context, NK_KEY_DOWN, down);
            break;
        case key::CURSOR_LEFT:
            nk_input_key(m_context, NK_KEY_LEFT, down);
            break;
        case key::CURSOR_RIGHT:
            nk_input_key(m_context, NK_KEY_RIGHT, down);
            break;
        case key::HOME:
            nk_input_key(m_context, NK_KEY_TEXT_START, down);
            break;
        case key::END:
            nk_input_key(m_context, NK_KEY_TEXT_END, down);
            break;
        default:
            break;
        }

        // Handle modifier keys
        if (m.test(modifier_value::SHIFT)) {
            nk_input_key(m_context, NK_KEY_SHIFT, down);
        }
        if (m.test(modifier_value::CONTROL)) {
            nk_input_key(m_context, NK_KEY_CTRL, down);
        }

        nk_input_end(m_context);
        return true;
    }

    bool immediate_ui::handle_mouse_action(uint32_t        button,
                                           mouse_action    action,
                                           const modifier& m,
                                           uint32_t        x,
                                           uint32_t        y)
    {
        nk_input_begin(m_context);

        bool down = (action == mouse_action::PRESS);

        // Map button index to Nuklear button enum
        nk_buttons nk_button;
        switch (button) {
        case 0:
            nk_button = NK_BUTTON_LEFT;
            break;
        case 1:
            nk_button = NK_BUTTON_RIGHT;
            break;
        case 2:
            nk_button = NK_BUTTON_MIDDLE;
            break;
        default:
            nk_input_end(m_context);
            return true;
        }

        nk_input_button(m_context,
                        nk_button,
                        static_cast<int>(x),
                        static_cast<int>(y),
                        down);

        nk_input_end(m_context);
        return true;
    }

    bool immediate_ui::handle_mouse_move(uint32_t x, uint32_t y)
    {
        nk_input_begin(m_context);
        nk_input_motion(m_context, static_cast<int>(x), static_cast<int>(y));
        nk_input_end(m_context);
        return true;
    }

    bool immediate_ui::handle_character(uint32_t ch)
    {
        nk_input_begin(m_context);
        nk_input_unicode(m_context, ch);
        nk_input_end(m_context);
        return true;
    }

    bool immediate_ui::handle_mouse_wheel(int32_t x, int32_t y)
    {
        nk_input_begin(m_context);
        struct nk_vec2 scroll;
        scroll.x = static_cast<float>(x);
        scroll.y = static_cast<float>(y);
        nk_input_scroll(m_context, scroll);
        nk_input_end(m_context);
        return true;
    }

} // namespace mge
