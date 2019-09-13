// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/command_list.hpp"
#include <variant>
#include <vector>

namespace mge {

    /**
     * An abstract platform independent command list.
     *
     * This command list just collects the commands issued, but does
     * not use any native functionality. Executing such a command list
     * may impose a performance penalty.
     */
    class MGE_GRAPHICS_EXPORT memory_command_list : public command_list
    {
    public:
        struct clear_data {
            rgba_color color;
        };

        struct clear_depth_data {
            double value;
        };

        typedef std::variant<clear_data,
                             clear_depth_data> element;

    public:
        /**
         * Constructor.
         * @param context render context
         */
        memory_command_list(render_context& context);

        /**
         * Destructor.
         */
        ~memory_command_list() = default;
    private:
        std::vector<element> m_elements;
    };
}
