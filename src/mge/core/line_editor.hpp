// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"

#include <atomic>
#include <optional>
#include <string>

namespace mge {

    /**
     * @brief A line editor.
     *
     * A line editor handles the input in the console.
     * Only one line editor can exist at the same time.
     *
     */
    class MGECORE_EXPORT line_editor
    {
    public:
        /**
         * @brief Construct line editor with empty prompt.
         */
        line_editor();
        /**
         * @brief Construct line editor with given prompt.
         *
         * @param prompt prompt to display.
         */
        line_editor(const std::string& prompt);

        /**
         * @brief Destroy the line editor object
         */
        ~line_editor();

        /**
         * @brief Read next line from line editor.
         *
         * @return line read
         */
        std::optional<std::string> line();

        /**
         * @brief Get current prompt.
         *
         * @return current prompt
         */
        const std::string& prompt() const;

        /**
         * @brief Set prompt.
         *
         * @param p new prompt
         */
        void set_prompt(const std::string& p);

        /**
         * @brief Set prompt.
         *
         * @param p new prompt
         */
        void set_prompt(const char* p);

    private:
        std::string             m_prompt;
        static std::atomic<int> s_instances;
    };

} // namespace mge