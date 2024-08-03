// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"
#include "common.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "mge/graphics/data_type.hpp"
#include "opengl.hpp"
#include "program.hpp"
#include "render_context.hpp"
#include "vertex_buffer.hpp"

namespace mge::opengl {

    command_list::command_list(render_context& ctx)
        : mge::command_list(ctx, false)
        , m_opengl_context(ctx)
    {}

    command_list::~command_list() { destroy(); }

    void command_list::viewport(const mge::viewport& vp)
    {
        m_commands.emplace_back(viewport_command{vp});
    }

    void command_list::scissor(const mge::rectangle& scissor)
    {
        m_commands.emplace_back(scissor_command{scissor});
    }

    void command_list::default_scissor()
    {
        m_commands.emplace_back(
            scissor_command{m_opengl_context.default_scissor()});
    }

    void command_list::clear(const rgba_color& c)
    {
        m_commands.emplace_back(clear_command{c});
    }

    void command_list::draw(const mge::draw_command& command)
    {
        command_list::draw_command c;
        c.program_name = gl_program(command.program());
        c.topology = gl_topology(command.topology());

        c.vao = 0;
        glCreateVertexArrays(1, &c.vao);
        CHECK_OPENGL_ERROR(glCreateVertexArrays);
        m_all_vaos.push_back(c.vao);
        glBindVertexArray(c.vao);
        CHECK_OPENGL_ERROR(glBindVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer(command.vertices()));
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ARRAY_BUFFER));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                     gl_index_buffer(command.indices()));
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER));

        const auto& layout = command.vertices()->layout();
        uint32_t    index = 0;
        for (const auto& f : layout.formats()) {
            glEnableVertexAttribArray(index);
            CHECK_OPENGL_ERROR(glEnableVertexAttribArray);
            switch (f.type()) {
            case mge::data_type::FLOAT:
                glVertexAttribPointer(index,
                                      f.size(),
                                      GL_FLOAT,
                                      GL_FALSE,
                                      0,
                                      nullptr);
                CHECK_OPENGL_ERROR(glVertexAttribPointer);
                break;
            default:
                MGE_THROW(opengl::error)
                    << "Unsupported vertex array element type " << f.type();
            }
        }
        glBindVertexArray(0);
        CHECK_OPENGL_ERROR(glBindVertexArray(0));

        c.element_count = (GLsizei)command.indices()->element_count();
        m_commands.emplace_back(c);
    }

    void command_list::destroy()
    {
        if (!m_all_vaos.empty()) {
            glDeleteVertexArrays(static_cast<GLsizei>(m_all_vaos.size()),
                                 m_all_vaos.data());
            TRACE_OPENGL_ERROR(glDeleteVertexArrays);
        }
    }

    void command_list::execute()
    {
        for (const auto& cmd : m_commands) {
            std::visit(
                [&](auto&& arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, clear_command>) {
                        glClearColor(arg.clear_color.r,
                                     arg.clear_color.g,
                                     arg.clear_color.b,
                                     arg.clear_color.a);
                        CHECK_OPENGL_ERROR(glClearColor);
                        glClear(GL_COLOR_BUFFER_BIT);
                        CHECK_OPENGL_ERROR(glClear(GL_COLOR_BUFFER_BIT));
                    } else if constexpr (std::is_same_v<T, scissor_command>) {
                        glEnable(GL_SCISSOR_TEST);
                        CHECK_OPENGL_ERROR(glEnable);
                        glScissor(arg.scissor.bottom_left().x,
                                  arg.scissor.bottom_left().y,
                                  arg.scissor.width(),
                                  arg.scissor.height());
                        CHECK_OPENGL_ERROR(glScissor);
                    } else if constexpr (std::is_same_v<T, viewport_command>) {
                        glViewport(arg.viewport.lower_left().x,
                                   arg.viewport.lower_left().y,
                                   static_cast<GLsizei>(arg.viewport.width),
                                   static_cast<GLsizei>(arg.viewport.height));
                        CHECK_OPENGL_ERROR(glViewport);
                        glDepthRange(arg.viewport.min_depth,
                                     arg.viewport.max_depth);

                    } else if constexpr (std::is_same_v<T, draw_command>) {
                        glUseProgram(arg.program_name);
                        CHECK_OPENGL_ERROR(glUseProgram);
                        glBindVertexArray(arg.vao);
                        CHECK_OPENGL_ERROR(glBindVertexArray);
                        glDrawElements(arg.topology,
                                       arg.element_count,
                                       GL_UNSIGNED_INT,
                                       nullptr);
                        CHECK_OPENGL_ERROR(glDrawElements(...));

                        glBindVertexArray(0);
                        CHECK_OPENGL_ERROR(glBindVertexArray(0));
                        glUseProgram(0);
                        CHECK_OPENGL_ERROR(glUseProgram(0));
                    }
                },
                cmd);
        }
    }

} // namespace mge::opengl