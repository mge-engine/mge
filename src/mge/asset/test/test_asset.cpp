// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_not_found.hpp"
#include "mge/asset/asset_type.hpp"
#include "test/googletest.hpp"

using namespace mge::string_literals;
using namespace std::string_literals;

TEST(asset, create)
{
    mge::path  sample_path("test");
    mge::asset a(sample_path);
    EXPECT_EQ(sample_path, a.path());
}

TEST(asset, literal)
{
    auto      a = "test"_asset;
    mge::path sample_path("test");
    EXPECT_EQ(sample_path, a.path());
}

TEST(asset, size)
{
    auto a = "file_asset.txt"_asset;
    EXPECT_EQ(8u, a.size());
}

TEST(asset, has_properties)
{
    auto a = "file_asset.txt"_asset;
    EXPECT_TRUE(a.has_properties());
}

TEST(asset, properties)
{
    auto a = "file_asset.txt"_asset;
    auto p = a.properties();
    EXPECT_EQ("salat", p->get<std::string>("wurst"));
}

TEST(asset, locate_throws)
{
    EXPECT_THROW(mge::asset::locate("foobar", "unknown/foobar"_asset_type),
                 mge::asset_not_found);
}
