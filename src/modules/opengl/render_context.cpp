// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/zero_memory.hpp"
#include "mge/core/log.hpp"
#include "mge/graphics/memory_command_list.hpp"
#include "error.hpp"
#include "shader.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"
#include "texture_2d.hpp"
#include "pipeline.hpp"
#include <boost/algorithm/string.hpp>
#include <cctype>
#include <variant>

MGE_USE_LOG(OPENGL);

namespace opengl {
#ifdef MGE_OS_WINDOWS
    render_context::render_context(window *w, bool debug)
        :mge::render_context(w)
        ,m_hwnd(w->hwnd())
        ,m_hdc(nullptr)
        ,m_hglrc(nullptr)
        ,m_debug(debug)
    {
        m_hdc = GetDC(m_hwnd);
        if (!m_hdc) {
            MGE_THROW_SYSCALL_FAILED(GetDC);
        }

        choose_pixel_format();
        create_glrc();
        init_gl3w();
        collect_opengl_info();
        install_debug_callback();
        clear_current();
    }

    render_context::~render_context()
    {
        if(m_hdc && m_hwnd) {
            ReleaseDC(m_hwnd, m_hdc);
        }
    }

    void
    render_context::choose_pixel_format()
    {
        PIXELFORMATDESCRIPTOR pixel_format_desc;
        mge::zero_memory(pixel_format_desc);
        pixel_format_desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pixel_format_desc.nVersion = 1;
        pixel_format_desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pixel_format_desc.iPixelType = PFD_TYPE_RGBA;
        pixel_format_desc.cColorBits = 32;
        pixel_format_desc.cAlphaBits = 8;
        pixel_format_desc.cDepthBits = 24;
        pixel_format_desc.cStencilBits = 8;
        pixel_format_desc.iLayerType = PFD_MAIN_PLANE;

        int pixel_format = ChoosePixelFormat(m_hdc, &pixel_format_desc);
        if (pixel_format == 0) {
            MGE_THROW_SYSCALL_FAILED(ChoosePixelFormat);
        }
        if (!SetPixelFormat(m_hdc, pixel_format, &pixel_format_desc)) {
            MGE_THROW_SYSCALL_FAILED(SetPixelFormat);
        }
    }

    void
    render_context::create_glrc()
    {
        m_hglrc = wglCreateContext(m_hdc);
        if (!m_hglrc) {
            MGE_THROW_SYSCALL_FAILED(wglCreateContext);
        }
        if (!wglMakeCurrent(m_hdc, m_hglrc)) {
            MGE_THROW_SYSCALL_FAILED(wglMakeCurrent);
        }
    }

    static bool s_gl3w_initialized = false;
    static mge::shader_language s_glsl_language;

    void
    render_context::init_gl3w()
    {
        if (!s_gl3w_initialized) {
            MGE_INFO_LOG(OPENGL) << "Initialize gl3w";
            auto rc = gl3wInit();
            if (rc) {
                MGE_THROW(mge::runtime_exception)
                          << "Initialization of gl3w library failed with rc " << rc;
            }
            s_gl3w_initialized = true;
        }
    }

    void
    render_context::collect_opengl_info()
    {
        static bool opengl_info_logged = false;
        if(!opengl_info_logged) {
            opengl_info_logged = true;
            int major_version = 0;
            int minor_version = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &major_version);
            glGetIntegerv(GL_MINOR_VERSION, &minor_version);
            MGE_INFO_LOG(OPENGL) << "OpenGL version: " << major_version << "." << minor_version;
            std::string glsl_version_str((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
            auto vit = glsl_version_str.begin();
            while(vit != glsl_version_str.end() && (*vit == '.' || std::isdigit(*vit))) {
                ++vit;
            }
            std::string plain_glsl_version(glsl_version_str.begin(), vit);
            s_glsl_language = mge::shader_language("glsl", mge::version(plain_glsl_version));
            MGE_INFO_LOG(OPENGL) << "Shading language version: " << glsl_version_str;
            std::string extension_string((const char *)glGetString(GL_EXTENSIONS));
            auto it = boost::make_split_iterator(extension_string, boost::token_finder(boost::is_space()));
            MGE_INFO_LOG(OPENGL) << "OpenGL extensions:";
            while(!it.eof()) {
                if (!it->empty()) {
                    MGE_INFO_LOG(OPENGL) << *it;
                    m_extensions.insert(std::string(it->begin(), it->end()));
                }
                ++it;
            }
        }
    }

    static void debug_callback(GLenum source,
                               GLenum type,
                               GLuint id,
                               GLenum severity,
                               GLsizei length,
                               const GLchar *message,
                               const void *user_param)
    {
        MGE_INFO_LOG(OPENGL) << (char *)message;
    }

    void
    render_context::install_debug_callback()
    {
        if (m_debug) {
            if (m_extensions.find("GL_ARB_debug_output") != m_extensions.end()) {
                MGE_INFO_LOG(OPENGL) << "Install debug callback";
                glDebugMessageCallback(&debug_callback, this);
            }
        }
    }

    void
    render_context::clear_current()
    {
        if(!wglMakeCurrent(nullptr, nullptr)) {
            MGE_THROW_SYSCALL_FAILED(wglMakeCurrent);
        }
    }

    void
    render_context::assign_thread()
    {
        if (!wglMakeCurrent(m_hdc, m_hglrc)) {
            MGE_THROW_SYSCALL_FAILED(wglMakeCurrent);
        }
    }

    void
    render_context::flush()
    {
        if(!SwapBuffers(m_hdc)) {
            MGE_THROW_SYSCALL_FAILED(SwapBuffers);
        }
    }
#endif

    mge::index_buffer_ref
    render_context::create_index_buffer(mge::data_type type,
                                        mge::usage usage,
                                        size_t element_count,
                                        void *initial_data)
    {
        return std::make_shared<index_buffer>(*this,
                                              type,
                                              usage,
                                              element_count,
                                              initial_data);
    }

    mge::vertex_buffer_ref 
    render_context::create_vertex_buffer(const mge::vertex_layout& layout,
                                         mge::usage usage,
                                         size_t element_count,
                                         void *initial_data)
    {
        return std::make_shared<vertex_buffer>(*this,
                                               layout,
                                               usage,
                                               element_count,
                                               initial_data);
    }

    mge::texture_2d_ref 
    render_context::create_texture_2d(const mge::image_ref& image)
    {
        return std::make_shared<texture_2d>(*this,
                                            image);
    }

    mge::texture_2d_ref
    render_context::create_texture_2d()
    {
        return std::make_shared<texture_2d>(*this);
    }

    void
    render_context::shader_languages(std::vector<mge::shader_language>& languages) const
    {
        languages.push_back(s_glsl_language);
        return;
    }

    mge::shader_ref
    render_context::create_shader(mge::shader_type type)
    {
        return std::make_shared<shader>(*this, type);
    }

    mge::pipeline_ref
    render_context::create_pipeline()
    {
        return std::make_shared<pipeline>(*this);
    }

    mge::command_list_ref
    render_context::create_command_list()
    {
        return std::make_shared<mge::memory_command_list>(*this);
    }


    template<class... Ts> struct visitor : Ts... { using Ts::operator()...; };
    template<class... Ts> visitor(Ts...) -> visitor<Ts...>;

    void
    render_context::execute(const mge::command_list_ref& commands)
    {
        commands->assert_same_context(*this);
        const mge::memory_command_list *memory_commands = 
            static_cast<const mge::memory_command_list *>(commands.get());
        await([&]{
            for(const auto& c : *memory_commands) {
                std::visit(visitor {
                    [](const auto& arg) {},
                    [&](const mge::memory_command_list::clear_data& d) {
                        glClearColor(d.color.r, d.color.g, d.color.b, d.color.a);
                        CHECK_OPENGL_ERROR(glClearColor);
                        glClear(GL_COLOR_BUFFER_BIT);
                        CHECK_OPENGL_ERROR(glClear(GL_COLOR_BUFFER_BIT));
                    }
                }, c);
            }
        });
    }



}
