// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset_type.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(asset_type, create)
{
    mge::asset_type t("image", "jpeg");
    EXPECT_EQ(std::string("image"), t.type());
    EXPECT_EQ(std::string("jpeg"), t.subtype());
}

TEST(asset_type, output)
{
    mge::asset_type t("image", "jpeg");
    mge::test_stream_output(t, "image/jpeg");
}

TEST(asset_type, literals)
{
    using namespace mge::literals;
    auto t1 = "image"_at;
    auto t2 = "image/jpeg"_at;

    EXPECT_EQ(mge::asset_type("image", ""), t1);
    EXPECT_EQ(mge::asset_type("image", "jpeg"), t2);
}
