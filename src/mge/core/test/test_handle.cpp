// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/handle.hpp"
#include "test/googletest.hpp"

namespace {
    struct mock_object
    {};

    struct mock_context
    {
        static mock_context* get(uint16_t)
        {
            return nullptr;
        }

        template <typename T> T* object(uint16_t, uint16_t, uint32_t)
        {
            return nullptr;
        }
    };

    using test_handle = mge::handle<mock_object, mock_context>;
} // namespace

TEST(handle, default_constructor)
{
    test_handle h;
    EXPECT_FALSE(static_cast<bool>(h));
}

TEST(handle, value_constructor)
{
    test_handle h(1, 2, 3);
    // context returns nullptr, so bool is false
    EXPECT_FALSE(static_cast<bool>(h));
}

TEST(handle, copy_constructor)
{
    test_handle h1(1, 2, 3);
    test_handle h2(h1);
    EXPECT_FALSE(static_cast<bool>(h2));
}

TEST(handle, move_constructor)
{
    test_handle h1(1, 2, 3);
    test_handle h2(std::move(h1));
    // h1 should now be invalid (0xFFFF, 0xFFFFFFFF)
    EXPECT_FALSE(static_cast<bool>(h1));
    EXPECT_FALSE(static_cast<bool>(h2));
}

TEST(handle, copy_assignment)
{
    test_handle h1(1, 2, 3);
    test_handle h2;
    h2 = h1;
    EXPECT_FALSE(static_cast<bool>(h2));
}

TEST(handle, move_assignment)
{
    test_handle h1(1, 2, 3);
    test_handle h2;
    h2 = std::move(h1);
    EXPECT_FALSE(static_cast<bool>(h1));
    EXPECT_FALSE(static_cast<bool>(h2));
}

TEST(handle, self_copy_assignment)
{
    test_handle h(1, 2, 3);
    h = h;
    EXPECT_FALSE(static_cast<bool>(h));
}

TEST(handle, self_move_assignment)
{
    test_handle h(1, 2, 3);
    h = std::move(h);
    EXPECT_FALSE(static_cast<bool>(h));
}

TEST(handle, get_returns_nullptr)
{
    test_handle h(1, 2, 3);
    EXPECT_EQ(nullptr, h.get());
}

TEST(handle, arrow_returns_nullptr)
{
    test_handle h(1, 2, 3);
    EXPECT_EQ(nullptr, h.operator->());
}

TEST(handle, format_invalid)
{
    test_handle h;
    auto        s = fmt::format("{}", h);
    EXPECT_EQ("Invalid handle", s);
}
