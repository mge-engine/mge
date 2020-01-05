// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/asset/asset.hpp"

using namespace mge::string_literals;

TEST(asset, create)
{
    mge::path sample_path("test");
    mge::asset a(sample_path);
    EXPECT_EQ(sample_path, a.path());
}

TEST(asset, literal)
{
    auto a = "test"_asset;
    mge::path sample_path("test");
    EXPECT_EQ(sample_path, a.path());
}

TEST(asset, size)
{
    auto a = "file_asset.txt"_asset;
    EXPECT_EQ(8u, a.size());
}
