// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/vertex_layout.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(vertex_layout, default_constructor)
{
    mge::vertex_layout l;
    EXPECT_EQ(0u, l.size());
}

TEST(vertex_layout, initializer_list)
{
    mge::vertex_layout l{mge::vertex_format(mge::data_type::FLOAT, 3),
                         mge::vertex_format(mge::data_type::FLOAT, 2)};
    EXPECT_EQ(2u, l.size());
}

TEST(vertex_layout, push_back)
{
    mge::vertex_layout l;
    l.push_back(mge::vertex_format(mge::data_type::FLOAT, 3));
    EXPECT_EQ(1u, l.size());
    auto e = l[0];
    EXPECT_EQ(mge::vertex_format(mge::data_type::FLOAT, 3), e.format);
}

TEST(vertex_layout, push_back_with_semantic)
{
    mge::vertex_layout l;
    l.push_back(mge::vertex_format(mge::data_type::FLOAT, 3),
                mge::attribute_semantic::POSITION);
    EXPECT_EQ(1u, l.size());
    auto e = l[0];
    EXPECT_EQ(mge::attribute_semantic::POSITION, e.semantic);
}

TEST(vertex_layout, binary_size)
{
    mge::vertex_layout l{mge::vertex_format(mge::data_type::FLOAT, 3),
                         mge::vertex_format(mge::data_type::FLOAT, 2)};
    EXPECT_EQ(5 * sizeof(float), l.binary_size());
}

TEST(vertex_layout, stride)
{
    mge::vertex_layout l{mge::vertex_format(mge::data_type::FLOAT, 3)};
    EXPECT_EQ(l.binary_size(), l.stride());
}

TEST(vertex_layout, offset)
{
    mge::vertex_layout l{mge::vertex_format(mge::data_type::FLOAT, 3),
                         mge::vertex_format(mge::data_type::FLOAT, 2)};
    EXPECT_EQ(0u, l.offset(0));
    EXPECT_EQ(3 * sizeof(float), l.offset(1));
}

TEST(vertex_layout, offset_out_of_range)
{
    mge::vertex_layout l{mge::vertex_format(mge::data_type::FLOAT, 3)};
    EXPECT_THROW(l.offset(5), mge::exception);
}

TEST(vertex_layout, copy)
{
    mge::vertex_layout l1{mge::vertex_format(mge::data_type::FLOAT, 3)};
    mge::vertex_layout l2(l1);
    EXPECT_EQ(l1, l2);
}

TEST(vertex_layout, equality)
{
    mge::vertex_layout l1{mge::vertex_format(mge::data_type::FLOAT, 3)};
    mge::vertex_layout l2{mge::vertex_format(mge::data_type::FLOAT, 3)};
    EXPECT_EQ(l1, l2);
}

TEST(vertex_layout, inequality)
{
    mge::vertex_layout l1{mge::vertex_format(mge::data_type::FLOAT, 3)};
    mge::vertex_layout l2{mge::vertex_format(mge::data_type::FLOAT, 2)};
    EXPECT_NE(l1, l2);
}

TEST(vertex_layout, iterator)
{
    mge::vertex_layout l{mge::vertex_format(mge::data_type::FLOAT, 3),
                         mge::vertex_format(mge::data_type::FLOAT, 2)};
    int                count = 0;
    for (auto it = l.begin(); it != l.end(); ++it) {
        ++count;
    }
    EXPECT_EQ(2, count);
}

TEST(vertex_layout, const_iterator)
{
    const mge::vertex_layout l{mge::vertex_format(mge::data_type::FLOAT, 3)};
    int                      count = 0;
    for (auto it = l.cbegin(); it != l.cend(); ++it) {
        ++count;
    }
    EXPECT_EQ(1, count);
}

TEST(vertex_layout, parse_single)
{
    auto l = mge::parse_vertex_layout("[FLOAT[3]]");
    EXPECT_EQ(1u, l.size());
    EXPECT_EQ(mge::vertex_format(mge::data_type::FLOAT, 3), l[0].format);
}

TEST(vertex_layout, parse_multiple)
{
    auto l = mge::parse_vertex_layout("[FLOAT[3], FLOAT[2]]");
    EXPECT_EQ(2u, l.size());
    EXPECT_EQ(mge::vertex_format(mge::data_type::FLOAT, 3), l[0].format);
    EXPECT_EQ(mge::vertex_format(mge::data_type::FLOAT, 2), l[1].format);
}

TEST(vertex_layout, parse_invalid_too_short)
{
    EXPECT_THROW(mge::parse_vertex_layout("x"), mge::exception);
}

TEST(vertex_layout, parse_invalid_no_open)
{
    EXPECT_THROW(mge::parse_vertex_layout("xx"), mge::exception);
}

TEST(vertex_layout, parse_invalid_no_close)
{
    EXPECT_THROW(mge::parse_vertex_layout("[x"), mge::exception);
}

TEST(vertex_layout, format)
{
    mge::vertex_layout l{mge::vertex_format(mge::data_type::FLOAT, 3)};
    std::ostringstream oss;
    oss << l;
    EXPECT_FALSE(oss.str().empty());
}

TEST(vertex_layout, at_bounds_check)
{
    mge::vertex_layout l{mge::vertex_format(mge::data_type::FLOAT, 3)};
    EXPECT_NO_THROW(l.at(0));
    EXPECT_THROW(l.at(5), std::out_of_range);
}
