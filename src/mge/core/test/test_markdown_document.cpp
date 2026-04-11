// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/markdown_document.hpp"

#include <gtest/gtest.h>
#include <sstream>

using mge::markdown_document;

TEST(markdown_document, heading)
{
    markdown_document doc;
    doc.heading(1, "Title");
    EXPECT_EQ("# Title\n\n", doc.str());
}

TEST(markdown_document, heading_levels)
{
    markdown_document doc;
    doc.heading(3, "Sub");
    EXPECT_EQ("### Sub\n\n", doc.str());
}

TEST(markdown_document, paragraph)
{
    markdown_document doc;
    doc.paragraph("Hello world.");
    EXPECT_EQ("Hello world.\n\n", doc.str());
}

TEST(markdown_document, code_block_with_language)
{
    markdown_document doc;
    doc.code_block("cpp", "int x = 0;");
    EXPECT_EQ("```cpp\nint x = 0;\n```\n\n", doc.str());
}

TEST(markdown_document, code_block_no_language)
{
    markdown_document doc;
    doc.code_block("int x = 0;");
    EXPECT_EQ("```\nint x = 0;\n```\n\n", doc.str());
}

TEST(markdown_document, blockquote_single_line)
{
    markdown_document doc;
    doc.blockquote("A wise man once said");
    EXPECT_EQ("> A wise man once said\n\n", doc.str());
}

TEST(markdown_document, blockquote_multi_line)
{
    markdown_document doc;
    doc.blockquote("Line one\nLine two");
    EXPECT_EQ("> Line one\n> Line two\n\n", doc.str());
}

TEST(markdown_document, horizontal_rule)
{
    markdown_document doc;
    doc.horizontal_rule();
    EXPECT_EQ("---\n\n", doc.str());
}

TEST(markdown_document, unordered_list)
{
    markdown_document doc;
    doc.unordered_list({"Alpha", "Beta", "Gamma"});
    EXPECT_EQ("- Alpha\n- Beta\n- Gamma\n\n", doc.str());
}

TEST(markdown_document, ordered_list)
{
    markdown_document doc;
    doc.ordered_list({"First", "Second", "Third"});
    EXPECT_EQ("1. First\n2. Second\n3. Third\n\n", doc.str());
}

TEST(markdown_document, table)
{
    markdown_document doc;
    doc.table({"Name", "Value"}, {{"a", "1"}, {"b", "2"}});
    EXPECT_EQ("| Name | Value |\n"
              "| --- | --- |\n"
              "| a | 1 |\n"
              "| b | 2 |\n\n",
              doc.str());
}

TEST(markdown_document, line_and_blank_line)
{
    markdown_document doc;
    doc.line("some text").blank_line().line("more text");
    EXPECT_EQ("some text\n\nmore text\n", doc.str());
}

TEST(markdown_document, bold)
{
    EXPECT_EQ("**strong**", markdown_document::bold("strong"));
}

TEST(markdown_document, italic)
{
    EXPECT_EQ("*emphasis*", markdown_document::italic("emphasis"));
}

TEST(markdown_document, strikethrough)
{
    EXPECT_EQ("~~removed~~", markdown_document::strikethrough("removed"));
}

TEST(markdown_document, inline_code)
{
    EXPECT_EQ("`code`", markdown_document::inline_code("code"));
}

TEST(markdown_document, link)
{
    EXPECT_EQ("[MGE](https://example.com)",
              markdown_document::link("MGE", "https://example.com"));
}

TEST(markdown_document, image)
{
    EXPECT_EQ("![logo](img.png)", markdown_document::image("logo", "img.png"));
}

TEST(markdown_document, fluent_chaining)
{
    markdown_document doc;
    doc.heading(1, "Report")
        .paragraph("Introduction text.")
        .heading(2, "Details")
        .code_block("cpp", "return 0;")
        .horizontal_rule();

    EXPECT_EQ("# Report\n\n"
              "Introduction text.\n\n"
              "## Details\n\n"
              "```cpp\nreturn 0;\n```\n\n"
              "---\n\n",
              doc.str());
}

TEST(markdown_document, stream_output)
{
    markdown_document doc;
    doc.paragraph("Hello");

    std::stringstream ss;
    ss << doc;
    EXPECT_EQ(doc.str(), ss.str());
}

TEST(markdown_document, inline_in_paragraph)
{
    markdown_document doc;
    doc.paragraph(markdown_document::bold("important") + " text");
    EXPECT_EQ("**important** text\n\n", doc.str());
}
