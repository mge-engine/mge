// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/markdown_document.hpp"

#include <iostream>

namespace mge {

    markdown_document::markdown_document(std::pmr::memory_resource* resource)
        : m_resource(resource)
        , m_buffer(resource)
    {}

    markdown_document& markdown_document::heading(unsigned int     level,
                                                  std::string_view text)
    {
        for (unsigned int i = 0; i < level; ++i) {
            m_buffer += '#';
        }
        m_buffer += ' ';
        m_buffer += text;
        m_buffer += "\n\n";
        return *this;
    }

    markdown_document& markdown_document::paragraph(std::string_view text)
    {
        m_buffer += text;
        m_buffer += "\n\n";
        return *this;
    }

    markdown_document& markdown_document::code_block(std::string_view language,
                                                     std::string_view code)
    {
        m_buffer += "```";
        m_buffer += language;
        m_buffer += "\n";
        m_buffer += code;
        m_buffer += "\n```\n\n";
        return *this;
    }

    markdown_document& markdown_document::code_block(std::string_view code)
    {
        m_buffer += "```\n";
        m_buffer += code;
        m_buffer += "\n```\n\n";
        return *this;
    }

    markdown_document& markdown_document::blockquote(std::string_view text)
    {
        std::string_view remaining = text;
        while (!remaining.empty()) {
            auto pos = remaining.find('\n');
            if (pos == std::string_view::npos) {
                m_buffer += "> ";
                m_buffer += remaining;
                m_buffer += "\n";
                break;
            }
            m_buffer += "> ";
            m_buffer += remaining.substr(0, pos);
            m_buffer += "\n";
            remaining = remaining.substr(pos + 1);
        }
        m_buffer += "\n";
        return *this;
    }

    markdown_document& markdown_document::horizontal_rule()
    {
        m_buffer += "---\n\n";
        return *this;
    }

    markdown_document& markdown_document::unordered_list(
        const std::pmr::vector<std::pmr::string>& items)
    {
        for (const auto& item : items) {
            m_buffer += "- ";
            m_buffer += item;
            m_buffer += "\n";
        }
        m_buffer += "\n";
        return *this;
    }

    markdown_document& markdown_document::ordered_list(
        const std::pmr::vector<std::pmr::string>& items)
    {
        size_t n = 1;
        for (const auto& item : items) {
            m_buffer += std::to_string(n);
            m_buffer += ". ";
            m_buffer += item;
            m_buffer += "\n";
            ++n;
        }
        m_buffer += "\n";
        return *this;
    }

    markdown_document& markdown_document::table(
        const std::pmr::vector<std::pmr::string>&                   headers,
        const std::pmr::vector<std::pmr::vector<std::pmr::string>>& rows)
    {
        m_buffer += "|";
        for (const auto& h : headers) {
            m_buffer += " ";
            m_buffer += h;
            m_buffer += " |";
        }
        m_buffer += "\n|";
        for (size_t i = 0; i < headers.size(); ++i) {
            m_buffer += " --- |";
        }
        m_buffer += "\n";
        for (const auto& row : rows) {
            m_buffer += "|";
            for (const auto& cell : row) {
                m_buffer += " ";
                m_buffer += cell;
                m_buffer += " |";
            }
            m_buffer += "\n";
        }
        m_buffer += "\n";
        return *this;
    }

    markdown_document& markdown_document::line(std::string_view text)
    {
        m_buffer += text;
        m_buffer += "\n";
        return *this;
    }

    markdown_document& markdown_document::text(std::string_view text)
    {
        m_buffer += text;
        return *this;
    }

    markdown_document& markdown_document::blank_line()
    {
        m_buffer += "\n";
        return *this;
    }

    std::pmr::string markdown_document::bold(std::string_view text) const
    {
        std::pmr::string result(m_resource);
        result.reserve(text.size() + 4);
        result += "**";
        result += text;
        result += "**";
        return result;
    }

    std::pmr::string markdown_document::italic(std::string_view text) const
    {
        std::pmr::string result(m_resource);
        result.reserve(text.size() + 2);
        result += "*";
        result += text;
        result += "*";
        return result;
    }

    std::pmr::string
    markdown_document::strikethrough(std::string_view text) const
    {
        std::pmr::string result(m_resource);
        result.reserve(text.size() + 4);
        result += "~~";
        result += text;
        result += "~~";
        return result;
    }

    std::pmr::string markdown_document::inline_code(std::string_view text) const
    {
        std::pmr::string result(m_resource);
        result.reserve(text.size() + 2);
        result += "`";
        result += text;
        result += "`";
        return result;
    }

    std::pmr::string markdown_document::link(std::string_view text,
                                             std::string_view url) const
    {
        std::pmr::string result(m_resource);
        result.reserve(text.size() + url.size() + 4);
        result += "[";
        result += text;
        result += "](";
        result += url;
        result += ")";
        return result;
    }

    std::pmr::string markdown_document::image(std::string_view alt,
                                              std::string_view url) const
    {
        std::pmr::string result(m_resource);
        result.reserve(alt.size() + url.size() + 5);
        result += "![";
        result += alt;
        result += "](";
        result += url;
        result += ")";
        return result;
    }

    std::pmr::string markdown_document::str() const
    {
        return std::pmr::string(m_buffer, m_resource);
    }

    std::ostream& operator<<(std::ostream& os, const markdown_document& doc)
    {
        os << std::string_view(doc.m_buffer);
        return os;
    }

} // namespace mge
