// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
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

    line_editor::line_editor(const std::string& prompt)
    {
        if (++s_instances > 1) {
            MGE_THROW(illegal_state)
                << "Only one line editor can be instantiated at same time";
        }
        m_prompt = prompt;
    }

    line_editor::~line_editor() { --s_instances; }

    std::optional<std::string> line_editor::line()
    {
        std::optional<std::string> result;

        char* l = linenoise(m_prompt.c_str());

        if (l != nullptr) {
            std::string result_str(l);
            free(l);
            result = result_str;
        }

        return result;
    }

    const std::string& line_editor::prompt() const { return m_prompt; }

    void line_editor::set_prompt(const std::string& prompt)
    {
        m_prompt = prompt;
    }

    void line_editor::set_prompt(const char* prompt)
    {
        m_prompt = prompt ? prompt : "";
    }

} // namespace mge