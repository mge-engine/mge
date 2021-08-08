// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(data_type, output)
{
    mge::data_type t = mge::data_type::UINT16;
    mge::test_stream_output(t, "uint16");
}

TEST(data_type, size)
{
    EXPECT_EQ(sizeof(float), mge::data_type_size(mge::data_type::FLOAT));
    EXPECT_EQ(sizeof(int), mge::data_type_size(mge::data_type::INT32));
    EXPECT_EQ(sizeof(int64_t), mge::data_type_size(mge::data_type::INT64));
}

TEST(data_type, parse)
{
    EXPECT_EQ(mge::data_type::INT8, mge::parse_data_type("int8"));
    EXPECT_EQ(mge::data_type::INT16, mge::parse_data_type("int16"));
    EXPECT_EQ(mge::data_type::INT32, mge::parse_data_type("int32"));
    EXPECT_EQ(mge::data_type::INT64, mge::parse_data_type("int64"));
    EXPECT_EQ(mge::data_type::INT128, mge::parse_data_type("int128"));
    EXPECT_EQ(mge::data_type::UINT8, mge::parse_data_type("uint8"));
    EXPECT_EQ(mge::data_type::UINT16, mge::parse_data_type("uint16"));
    EXPECT_EQ(mge::data_type::UINT32, mge::parse_data_type("uint32"));
    EXPECT_EQ(mge::data_type::UINT64, mge::parse_data_type("uint64"));
    EXPECT_EQ(mge::data_type::UINT128, mge::parse_data_type("uint128"));
    EXPECT_EQ(mge::data_type::HALF, mge::parse_data_type("half"));
    EXPECT_EQ(mge::data_type::FLOAT, mge::parse_data_type("float"));
    EXPECT_EQ(mge::data_type::DOUBLE, mge::parse_data_type("double"));
    EXPECT_EQ(mge::data_type::LONG_DOUBLE, mge::parse_data_type("long double"));
}