#pragma once
#include "mge/graphics/command_sequence.hpp"
#include "opengl.hpp"

#include "mge/core/small_vector.hpp"
#include <variant>
#include <vector>

namespace mge::opengl {

    class render_context;

    class command_sequence : public mge::command_sequence
    {
    public:
        command_sequence(render_context& context);
        ~command_sequence() override;

        virtual void clear(const rgba_color& c) override;
        virtual void draw(const mge::draw_command& command) override;

    private:
        void delete_all_vaos();

        struct draw_command
        {
            GLuint  program_name;
            GLuint  vao;
            GLuint  topology;
            GLsizei element_count;
        };

        using opengl_command = std::variant<std::monostate, draw_command>;
        using opengl_command_vector = std::vector<opengl_command>;

        opengl_command_vector         m_commands;
        mge::small_vector<GLuint, 10> m_all_vaos;
    };

} // namespace mge::opengl