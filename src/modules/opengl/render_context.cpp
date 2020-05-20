// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "mge/core/log.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/zero_memory.hpp"
#include "mge/graphics/memory_command_list.hpp"
#include "pipeline.hpp"
#include "shader_program.hpp"
#include "texture_2d.hpp"
#include "vertex_buffer.hpp"
#include <boost/algorithm/string.hpp>
#include <cctype>
#include <variant>

MGE_USE_LOG(OPENGL);

namespace opengl {
#ifdef MGE_OS_WINDOWS
    render_context::render_context(mge::render_system *system, window *w,
                                   bool debug)
        : mge::render_context(system, w), m_hwnd(w->hwnd()), m_hdc(nullptr),
          m_hglrc(nullptr), m_debug(debug)
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
        clear_vaos();
        if (m_hdc && m_hwnd) {
            ReleaseDC(m_hwnd, m_hdc);
        }
    }

    void render_context::clear_vaos()
    {
        for (const auto &e : m_vaos) {
            glDeleteVertexArrays(1, &e.second);
            LOG_OPENGL_ERROR(glDeleteVertexArrays);
        }
    }

    GLuint render_context::create_vao(const mge::draw_command &cmd)
    {
        GLuint vao = 0;
        glCreateVertexArrays(1, &vao);
        CHECK_OPENGL_ERROR(glCreateVertexArrays);
        glBindVertexArray(vao);
        CHECK_OPENGL_ERROR(glBindVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer(cmd.vertices()));
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ARRAY_BUFFER));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer(cmd.indices()));
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER));

        const auto &layout = cmd.vertices()->layout();
        uint32_t    index  = 0;
        for (const auto &f : layout) {
            glEnableVertexAttribArray(index);
            CHECK_OPENGL_ERROR(glEnableVertexAttribArray);
            switch (f.type()) {
            case mge::data_type::FLOAT_VEC3:
                glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
                CHECK_OPENGL_ERROR(glVertexAttribPointer);
                break;
            default:
                MGE_THROW(opengl::error)
                    << "Unsupported vertex array element type " << f.type();
            }
        }
        glBindVertexArray(0);
        CHECK_OPENGL_ERROR(glBindVertexArray(0));
        vao_key_type k(gl_program(cmd.pipeline()),
                       gl_vertex_buffer(cmd.vertices()),
                       gl_index_buffer(cmd.indices()));
        m_vaos[k] = vao;
        return vao;
    }

    GLuint render_context::lookup_vao(const mge::draw_command &cmd)
    {
        vao_key_type k(gl_program(cmd.pipeline()),
                       gl_vertex_buffer(cmd.vertices()),
                       gl_index_buffer(cmd.indices()));
        auto         it = m_vaos.find(k);
        return it == m_vaos.end() ? 0 : it->second;
    }

    void render_context::clear_vaos_of_program(GLuint program)
    {
        vao_key_type k(program, 0, 0);
        auto         it = m_vaos.lower_bound(k);
        while (it != m_vaos.end() && std::get<0>(it->first) == program) {
            glDeleteVertexArrays(1, &it->second);
            LOG_OPENGL_ERROR(glDeleteVertexArrays);
            it = m_vaos.erase(it);
        }
    }

    void render_context::choose_pixel_format()
    {
        PIXELFORMATDESCRIPTOR pixel_format_desc;
        mge::zero_memory(pixel_format_desc);
        pixel_format_desc.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
        pixel_format_desc.nVersion = 1;
        pixel_format_desc.dwFlags =
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pixel_format_desc.iPixelType   = PFD_TYPE_RGBA;
        pixel_format_desc.cColorBits   = 32;
        pixel_format_desc.cAlphaBits   = 8;
        pixel_format_desc.cDepthBits   = 24;
        pixel_format_desc.cStencilBits = 8;
        pixel_format_desc.iLayerType   = PFD_MAIN_PLANE;

        int pixel_format = ChoosePixelFormat(m_hdc, &pixel_format_desc);
        if (pixel_format == 0) {
            MGE_THROW_SYSCALL_FAILED(ChoosePixelFormat);
        }
        if (!SetPixelFormat(m_hdc, pixel_format, &pixel_format_desc)) {
            MGE_THROW_SYSCALL_FAILED(SetPixelFormat);
        }
    }

    void render_context::create_glrc()
    {
        m_hglrc = wglCreateContext(m_hdc);
        if (!m_hglrc) {
            MGE_THROW_SYSCALL_FAILED(wglCreateContext);
        }
        if (!wglMakeCurrent(m_hdc, m_hglrc)) {
            MGE_THROW_SYSCALL_FAILED(wglMakeCurrent);
        }
    }

    static bool                 s_gl3w_initialized = false;
    static mge::shader_language s_glsl_language;

    void render_context::init_gl3w()
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

    void render_context::collect_opengl_info()
    {
        static bool opengl_info_logged = false;
        if (!opengl_info_logged) {
            opengl_info_logged = true;
            int major_version  = 0;
            int minor_version  = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &major_version);
            glGetIntegerv(GL_MINOR_VERSION, &minor_version);
            MGE_INFO_LOG(OPENGL)
                << "OpenGL version: " << major_version << "." << minor_version;
            std::string glsl_version_str(
                (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
            auto vit = glsl_version_str.begin();
            while (vit != glsl_version_str.end() &&
                   (*vit == '.' || std::isdigit(*vit))) {
                ++vit;
            }
            std::string plain_glsl_version(glsl_version_str.begin(), vit);
            s_glsl_language =
                mge::shader_language("glsl", mge::version(plain_glsl_version));
            MGE_INFO_LOG(OPENGL)
                << "Shading language version: " << glsl_version_str;
            std::string extension_string(
                (const char *)glGetString(GL_EXTENSIONS));
            auto it = boost::make_split_iterator(
                extension_string, boost::token_finder(boost::is_space()));
            MGE_INFO_LOG(OPENGL) << "OpenGL extensions:";
            while (!it.eof()) {
                if (!it->empty()) {
                    MGE_INFO_LOG(OPENGL) << *it;
                    m_extensions.insert(std::string(it->begin(), it->end()));
                }
                ++it;
            }
        }
    }

    static void debug_callback(GLenum source, GLenum type, GLuint id,
                               GLenum severity, GLsizei length,
                               const GLchar *message, const void *user_param)
    {
        MGE_INFO_LOG(OPENGL) << (char *)message;
    }

    void render_context::install_debug_callback()
    {
        if (m_debug) {
            if (m_extensions.find("GL_ARB_debug_output") !=
                m_extensions.end()) {
                MGE_INFO_LOG(OPENGL) << "Install debug callback";
                glEnable(GL_DEBUG_OUTPUT);
                CHECK_OPENGL_ERROR(glEnable(GL_DEBUG_OUTPUT));
                glDebugMessageCallback(&debug_callback, this);
                CHECK_OPENGL_ERROR(glDebugMessageCallback);
            }
        }
    }

    void render_context::clear_current()
    {
        if (!wglMakeCurrent(nullptr, nullptr)) {
            MGE_THROW_SYSCALL_FAILED(wglMakeCurrent);
        }
    }

    void render_context::assign_thread()
    {
        if (!wglMakeCurrent(m_hdc, m_hglrc)) {
            MGE_THROW_SYSCALL_FAILED(wglMakeCurrent);
        }
    }

    void render_context::flush()
    {
        if (!SwapBuffers(m_hdc)) {
            MGE_WARNING_LOG(OPENGL) << "SwapBuffers failed";
        }
    }
#endif

    mge::index_buffer_ref
    render_context::create_index_buffer(mge::data_type type, mge::usage usage,
                                        size_t element_count,
                                        void * initial_data)
    {
        return std::make_shared<index_buffer>(*this, type, usage, element_count,
                                              initial_data);
    }

    mge::vertex_buffer_ref
    render_context::create_vertex_buffer(const mge::vertex_layout &layout,
                                         mge::usage usage, size_t element_count,
                                         void *initial_data)
    {
        return std::make_shared<vertex_buffer>(*this, layout, usage,
                                               element_count, initial_data);
    }

    mge::texture_2d_ref
    render_context::create_texture_2d(const mge::image_ref &image)
    {
        return std::make_shared<texture_2d>(*this, image);
    }

    mge::texture_2d_ref render_context::create_texture_2d()
    {
        return std::make_shared<texture_2d>(*this);
    }

    void render_context::shader_languages(
        std::vector<mge::shader_language> &languages) const
    {
        languages.push_back(s_glsl_language);
        return;
    }

    mge::shader_program_ref
    render_context::create_shader_program(mge::shader_type type)
    {
        return std::make_shared<shader_program>(*this, type);
    }

    mge::pipeline_ref render_context::create_pipeline()
    {
        return std::make_shared<pipeline>(*this);
    }

    mge::command_list_ref render_context::create_command_list()
    {
        return std::make_shared<mge::memory_command_list>(*this);
    }

    void render_context::draw_command(const mge::draw_command &cmd)
    {
        auto vao = lookup_vao(cmd);
        if (!vao) {
            vao = create_vao(cmd);
        }
        glUseProgram(gl_program(cmd.pipeline()));
        CHECK_OPENGL_ERROR(glUseProgram);
        glBindVertexArray(vao);
        CHECK_OPENGL_ERROR(glBindVertexArray);

        glDrawElements(GL_TRIANGLES, (GLsizei)cmd.indices()->element_count(),
                       GL_UNSIGNED_INT, nullptr);
        CHECK_OPENGL_ERROR(glDrawElements(GL_TRIANGLES, ...));

        glBindVertexArray(0);
        CHECK_OPENGL_ERROR(glBindVertexArray(0));
        glUseProgram(0);
        CHECK_OPENGL_ERROR(glUseProgram(0));
    }

    template <class... Ts> struct visitor : Ts...
    {
        using Ts::operator()...;
    };
    template <class... Ts> visitor(Ts...) -> visitor<Ts...>;

    void render_context::execute(const mge::command_list_ref &commands)
    {
        commands->assert_same_context(*this);
        const mge::memory_command_list *memory_commands =
            static_cast<const mge::memory_command_list *>(commands.get());
        await([&] {
            for (const auto &c : *memory_commands) {
                std::visit(
                    visitor{[](const auto &arg) {},
                            [&](const mge::memory_command_list::clear_data &d) {
                                glClearColor(d.color.r, d.color.g, d.color.b,
                                             d.color.a);
                                CHECK_OPENGL_ERROR(glClearColor);
                                glClear(GL_COLOR_BUFFER_BIT);
                                CHECK_OPENGL_ERROR(
                                    glClear(GL_COLOR_BUFFER_BIT));
                            },
                            [&](const mge::memory_command_list::draw_data d) {
                                draw_command(d.cmd);
                            }},
                    c);
            }
        });
    }

} // namespace opengl
