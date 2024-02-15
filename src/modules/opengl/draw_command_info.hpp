#pragma once
#include "opengl.hpp"

namespace mge::opengl {
    struct draw_command_info
    {
        draw_command_info() = default;
        draw_command_info(const draw_command_info&) = delete;
        draw_command_info& operator=(const draw_command_info&) = delete;

        draw_command_info(draw_command_info&& other)
        {
            program_name = other.program_name;
            vao = other.vao;
            topology = other.topology;
            element_count = other.element_count;
            other.vao = 0;
        }

        draw_command_info& operator=(draw_command_info&& other)
        {
            program_name = other.program_name;
            vao = other.vao;
            topology = other.topology;
            element_count = other.element_count;
            other.vao = 0;
            return *this;
        }

        ~draw_command_info();
        GLuint  program_name;
        GLuint  vao;
        GLuint  topology;
        GLsizei element_count;
    };
}