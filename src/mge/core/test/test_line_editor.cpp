// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/line_editor.hpp"
#include "mge/core/stdexceptions.hpp"
#include "test/googletest.hpp"

TEST(line_editor, default_constructor)
{
    mge::line_editor editor;
    EXPECT_TRUE(editor.prompt().empty());
}

TEST(line_editor, constructor_with_prompt)
{
    mge::line_editor editor(">>> ");
    EXPECT_EQ(">>> ", editor.prompt());
}

TEST(line_editor, set_prompt_string)
{
    mge::line_editor editor;
    editor.set_prompt("test> ");
    EXPECT_EQ("test> ", editor.prompt());
}

TEST(line_editor, set_prompt_cstr)
{
    mge::line_editor editor;
    editor.set_prompt("foo> ");
    EXPECT_EQ("foo> ", editor.prompt());
}

TEST(line_editor, singleton_after_destroy)
{
    {
        mge::line_editor editor1;
    }
    mge::line_editor editor2;
    EXPECT_TRUE(editor2.prompt().empty());
}

TEST(line_editor, singleton_enforcement)
{
    mge::line_editor editor1;
    EXPECT_THROW(mge::line_editor editor2, mge::illegal_state);
}
