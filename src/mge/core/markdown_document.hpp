// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"

#include <iosfwd>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace mge {

    /**
     * Simple markdown document builder.
     */
    class MGECORE_EXPORT markdown_document
    {
    public:
        markdown_document();
        ~markdown_document() = default;

        markdown_document& heading(unsigned int level, std::string_view text);
        markdown_document& paragraph(std::string_view text);
        markdown_document& code_block(std::string_view language,
                                      std::string_view code);
        markdown_document& code_block(std::string_view code);
        markdown_document& blockquote(std::string_view text);
        markdown_document& horizontal_rule();

        markdown_document&
        unordered_list(const std::vector<std::string>& items);
        markdown_document& ordered_list(const std::vector<std::string>& items);

        markdown_document&
        table(const std::vector<std::string>&              headers,
              const std::vector<std::vector<std::string>>& rows);

        markdown_document& line(std::string_view text);
        markdown_document& blank_line();

        static std::string bold(std::string_view text);
        static std::string italic(std::string_view text);
        static std::string strikethrough(std::string_view text);
        static std::string inline_code(std::string_view text);
        static std::string link(std::string_view text, std::string_view url);
        static std::string image(std::string_view alt, std::string_view url);

        std::string str() const;

        friend MGECORE_EXPORT std::ostream&
        operator<<(std::ostream& os, const markdown_document& doc);

    private:
        std::stringstream m_buffer;
    };

} // namespace mge
