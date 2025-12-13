// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "command_list.hpp"
#include "index_buffer.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/zero_memory.hpp"
#include "program.hpp"
#include "shader.hpp"
#include "swap_chain.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"

namespace mge {
    MGE_USE_TRACE(OPENGL);
}

namespace mge::opengl {
#ifdef MGE_OS_WINDOWS
    render_context::render_context(mge::opengl::window* context_window)
        : m_window(context_window)
        , m_hwnd(context_window->hwnd())
        , m_hdc(0)
        , m_primary_hglrc(0)
        , m_lock("opengl::render_context")
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
            MGE_INFO_TRACE(OPENGL, "Initializing gl3w");
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
        MGE_INFO_TRACE(OPENGL, "Collecting OpenGL information");
        s_glinfo.ptr();
    }

#else
#    error Missing port
#endif

    render_context::~render_context()
    {
        m_frame_command_lists.clear();
        m_command_lists.clear();
        m_programs.clear();
        m_shaders.clear();
        m_vertex_buffers.clear();
        m_index_buffers.clear();
    }

    singleton<opengl_info> render_context::s_glinfo;

    mge::index_buffer* render_context::create_index_buffer(mge::data_type dt,
                                                           size_t data_size,
                                                           void*  data)
    {
        auto result =
            std::make_unique<index_buffer>(*this, dt, data_size, data);
        auto ptr = result.get();
        m_index_buffers[ptr] = std::move(result);
        return ptr;
    }

    void render_context::destroy_index_buffer(mge::index_buffer* ib)
    {
        auto it = m_index_buffers.find(ib);
        if (it != m_index_buffers.end()) {
            m_index_buffers.erase(it);
        } else {
            MGE_THROW(illegal_state)
                << "Attempt to destroy unknown index buffer";
        }
    }

    mge::vertex_buffer* render_context::create_vertex_buffer(
        const mge::vertex_layout& layout, size_t data_size, void* data)
    {
        auto result =
            std::make_unique<vertex_buffer>(*this, layout, data_size, data);
        auto ptr = result.get();
        m_vertex_buffers[ptr] = std::move(result);
        return ptr;
    }

    void render_context::destroy_vertex_buffer(mge::vertex_buffer* vb)
    {
        auto it = m_vertex_buffers.find(vb);
        if (it != m_vertex_buffers.end()) {
            m_vertex_buffers.erase(it);
        } else {
            MGE_THROW(illegal_state)
                << "Attempt to destroy unknown vertex buffer";
        }
    }

    mge::shader* render_context::create_shader(mge::shader_type t)
    {
        auto result = std::make_unique<shader>(*this, t);
        auto ptr = result.get();
        m_shaders[ptr] = std::move(result);
        return ptr;
    }

    void render_context::destroy_shader(mge::shader* s)
    {
        auto it = m_shaders.find(s);
        if (it != m_shaders.end()) {
            m_shaders.erase(it);
        } else {
            MGE_THROW(illegal_state) << "Attempt to destroy unknown shader";
        }
    }

    mge::program* render_context::create_program()
    {
        auto result = std::make_unique<program>(*this);
        auto ptr = result.get();
        m_programs[ptr] = std::move(result);
        return ptr;
    }

    void render_context::destroy_program(mge::program* p)
    {
        auto it = m_programs.find(p);
        if (it != m_programs.end()) {
            m_programs.erase(it);
        } else {
            MGE_THROW(illegal_state) << "Attempt to destroy unknown program";
        }
    }

    mge::command_list* render_context::create_command_list()
    {
        auto  ptr = std::make_unique<command_list>(*this);
        auto* result = ptr.get();
        m_command_lists[result] = std::move(ptr);
        return result;
    }

    void render_context::destroy_command_list(mge::command_list* cl)
    {
        m_command_lists.erase(cl);
    }

    mge::frame_command_list* render_context::create_current_frame_command_list()
    {
        auto* result = mge::render_context::create_current_frame_command_list();
        m_frame_command_lists[result] =
            std::unique_ptr<mge::frame_command_list>(result);
        return result;
    }

    void
    render_context::destroy_frame_command_list(mge::frame_command_list* fcl)
    {
        m_frame_command_lists.erase(fcl);
    }

    mge::texture_ref render_context::create_texture(mge::texture_type type)
    {
        mge::texture_ref result = std::make_shared<texture>(*this, type);
        return result;
    }

    mge::rectangle render_context::default_scissor() const
    {
        return mge::rectangle(m_window->position(), m_window->extent());
    }

    uint32_t render_context::window_height() const
    {
        return m_window->extent().height;
    }

} // namespace mge::opengl