#pragma once
#include "mge/graphics/dllexport.hpp"

#include "mge/graphics/draw_command.hpp"
#include "mge/graphics/rgba_color.hpp"
#include <variant>
#include <vector>

namespace mge {

    /**
     * @brief Sequence of commands.
     */
    class MGEGRAPHICS_EXPORT command_sequence
    {
    public:
        struct clear_command
        {
            rgba_color clear_color;
        };

        struct draw_command
        {
            mge::draw_command command;
        };

        using command =
            std::variant<std::monostate, clear_command, draw_command>;

        command_sequence() = default;
        command_sequence(const command_sequence&) = default;
        command_sequence(command_sequence&&) = default;
        ~command_sequence() = default;

        void clear(const rgba_color& c);
        void draw(const mge::draw_command& command);

    private:
        using command_vector = std::vector<command>;
        command_vector m_commands;
    };
} // namespace mge