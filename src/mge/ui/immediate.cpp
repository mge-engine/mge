// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/ui/immediate.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"

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
        (void)old;
        if (size == 0) {
            return nullptr;
        }
        return mge::malloc(size);
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

} // namespace mge
