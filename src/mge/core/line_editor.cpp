#include "mge/core/line_editor.hpp"
#include "mge/core/stdexceptions.hpp"

#include <linenoise.h>

namespace mge {
    std::atomic<int> line_editor::s_instances;

    line_editor::line_editor()
    {
        if (++s_instances > 1) {
            MGE_THROW(illegal_state)
                << "Only one line editor can be instantiated at same time";
        }
    }

    line_editor::~line_editor() { --s_instances; }

    std::string line_editor::line()
    {
        char* l = linenoise(">");

        std::string result(l);
        free(l);

        return result;
    }
} // namespace mge