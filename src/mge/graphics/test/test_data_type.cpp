// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(data_type, output)
{
    mge::data_type t = mge::data_type::UINT16;
    mge::test_stream_output(t, "UINT16");
}

TEST(data_type, size)
{
    EXPECT_EQ(sizeof(float), mge::data_type_size(mge::data_type::FLOAT));
    EXPECT_EQ(sizeof(int), mge::data_type_size(mge::data_type::INT32));
    EXPECT_EQ(sizeof(int64_t), mge::data_type_size(mge::data_type::INT64));
}

TEST(data_type, literal)
{
    using namespace mge::literals;

    EXPECT_EQ(mge::data_type::FLOAT, "FLOAT"_type);
}