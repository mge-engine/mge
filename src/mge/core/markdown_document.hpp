// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/markdown.hpp"

#include <iosfwd>
#include <memory_resource>
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
        markdown_document(std::pmr::memory_resource* resource =
                              std::pmr::get_default_resource());
        ~markdown_document() = default;

        markdown_document& heading(unsigned int level, std::string_view text);
        markdown_document& paragraph(std::string_view text);
        markdown_document& code_block(std::string_view language,
                                      std::string_view code);
        markdown_document& code_block(std::string_view code);
        markdown_document& blockquote(std::string_view text);
        markdown_document& horizontal_rule();

        markdown_document&
        unordered_list(const std::pmr::vector<std::pmr::string>& items);
        markdown_document&
        ordered_list(const std::pmr::vector<std::pmr::string>& items);

        markdown_document&
        table(const std::pmr::vector<std::pmr::string>& headers,
              const std::pmr::vector<std::pmr::vector<std::pmr::string>>& rows);

        markdown_document& line(std::string_view text);
        markdown_document& text(std::string_view text);
        markdown_document& blank_line();

        std::pmr::string bold(std::string_view text) const;
        std::pmr::string italic(std::string_view text) const;
        std::pmr::string strikethrough(std::string_view text) const;
        std::pmr::string inline_code(std::string_view text) const;
        std::pmr::string link(std::string_view text,
                              std::string_view url) const;
        std::pmr::string image(std::string_view alt,
                               std::string_view url) const;

        std::pmr::string str() const;

        friend MGECORE_EXPORT std::ostream&
        operator<<(std::ostream& os, const markdown_document& doc);

    private:
        std::pmr::memory_resource* m_resource;
        std::pmr::string           m_buffer;
    };

} // namespace mge
