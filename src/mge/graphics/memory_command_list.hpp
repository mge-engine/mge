#pragma once
#include "mge/graphics/command_list.hpp"
#include <boost/variant.hpp>
#include <vector>

namespace mge {

    class MGE_GRAPHICS_EXPORT memory_command_list : public command_list
    {
    public:
        struct clear_data {
            rgba_color color;
        };

        struct clear_depth_data {
            double value;
        };

        typedef boost::variant<clear_data,
                               clear_depth_data> element;

    public:
        memory_command_list(render_context& context);
        ~memory_command_list();
    private:
        std::vector<element> m_elements;
    };
}
