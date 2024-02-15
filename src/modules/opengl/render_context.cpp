// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "command_list.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/zero_memory.hpp"
#include "program.hpp"
#include "shader.hpp"
#include "swap_chain.hpp"
#include "vertex_buffer.hpp"
namespace mge {

    MGE_USE_TRACE(OPENGL);
}

namespace mge::opengl {
#ifdef MGE_OS_WINDOWS
    render_context::render_context(mge::opengl::window* context_window)
        : m_hwnd(context_window->hwnd())
        , m_hdc(0)
        , m_primary_hglrc(0)
        , m_lock("opengl::render_context")
        , m_draw_command_cache(256)
    {
        m_hdc = GetDC(m_hwnd);
        if (!m_hdc) {
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(GetDC);
        }
        select_pixel_format();
        create_primary_glrc();
        init_gl3w();
        collect_opengl_info();
    }

    void render_context::select_pixel_format()
    {
        PIXELFORMATDESCRIPTOR pixel_format_desc;
        mge::zero_memory(pixel_format_desc);

        pixel_format_desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pixel_format_desc.nVersion = 1;
        pixel_format_desc.dwFlags =
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pixel_format_desc.iPixelType = PFD_TYPE_RGBA;
        pixel_format_desc.cColorBits = 32;
        pixel_format_desc.cAlphaBits = 8;
        pixel_format_desc.cDepthBits = 24;
        pixel_format_desc.cStencilBits = 8;
        pixel_format_desc.iLayerType = PFD_MAIN_PLANE;

        int pixel_format = ChoosePixelFormat(m_hdc, &pixel_format_desc);
        if (pixel_format == 0) {
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(ChoosePixelFormat);
        }
        if (!SetPixelFormat(m_hdc, pixel_format, &pixel_format_desc)) {
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(SetPixelFormat);
        }
    }

    void render_context::create_primary_glrc()
    {
        HGLRC hglrc = wglCreateContext(m_hdc);
        if (!hglrc) {
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(wglCreateContext);
        }
        if (!wglMakeCurrent(m_hdc, hglrc)) {
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(wglMakeCurrent);
        }
        {
            std::scoped_lock<mge::mutex> scope(m_lock);
            m_thread_glrcs[mge::this_thread::get_id()] = hglrc;
        }
        m_primary_hglrc = hglrc;
    }

    static bool s_gl3w_initialized = false;

    void render_context::init_gl3w()
    {
        if (!s_gl3w_initialized) {
            MGE_INFO_TRACE(OPENGL) << "Initializing gl3w";
            auto rc = gl3wInit();
            if (rc) {
                MGE_THROW(runtime_exception)
                    << MGE_CALLED_FUNCTION(gl3wInit)
                    << "Initializing gl32 library failed: " << rc;
            }
            s_gl3w_initialized = true;
        }
    }

    void render_context::create_swap_chain()
    {
        m_swap_chain = std::make_shared<mge::opengl::swap_chain>(*this);
    }

    void render_context::collect_opengl_info()
    {
        MGE_INFO_TRACE(OPENGL) << "Collecting OpenGL information";
        s_glinfo.ptr();
    }

#else
#    error Missing port
#endif

    render_context::~render_context() {}

    singleton<opengl_info> render_context::s_glinfo;

    mge::index_buffer_ref render_context::create_index_buffer(mge::data_type dt,
                                                              size_t data_size,
                                                              void*  data)
    {
        mge::index_buffer_ref result =
            std::make_shared<index_buffer>(*this, dt, data_size, data);
        return result;
    }

    mge::vertex_buffer_ref render_context::create_vertex_buffer(
        const mge::vertex_layout& layout, size_t data_size, void* data)
    {
        mge::vertex_buffer_ref result =
            std::make_shared<vertex_buffer>(*this, layout, data_size, data);
        return result;
    }

    mge::shader_ref render_context::create_shader(mge::shader_type t)
    {
        mge::shader_ref result = std::make_shared<shader>(*this, t);
        return result;
    }

    mge::program_ref render_context::create_program()
    {
        mge::program_ref result = std::make_shared<program>(*this);
        return result;
    }

    mge::command_list_ref render_context::create_command_list()
    {
        mge::command_list_ref result = std::make_shared<command_list>(*this);
        return result;
    }

    void render_context::execute(const mge::command_sequence& sequence)
    {
        sequence.for_each([&](auto&& cmd) {
            using command_t = std::decay_t<decltype(cmd)>;
            if constexpr (std::is_same_v<
                              command_t,
                              mge::command_sequence::clear_command>) {
                glClearColor(cmd.clear_color.r,
                             cmd.clear_color.g,
                             cmd.clear_color.b,
                             cmd.clear_color.a);
                glClear(GL_COLOR_BUFFER_BIT);
            } else if constexpr (std::is_same_v<command_t, draw_command>) {
                execute_draw_command(cmd);
            } else {
                MGE_THROW(opengl::error)
                    << "Unsupported command type " << typeid(cmd).name();
            }
        });
    }

    void render_context::execute_draw_command(
        const mge::command_sequence::draw_command& cmd)
    {
#if 0
        auto cmd_info = m_draw_command_cache.get(&cmd);
        if (!cmd_info.has_value()) {
            cmd_info = draw_command_info();
            cmd_info->program_name = gl_program(cmd.program());
            m_draw_command_cache.put(&cmd, cmd_info.value());
        }
#endif
    }

} // namespace mge::opengl