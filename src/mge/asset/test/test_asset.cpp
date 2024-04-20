// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "asset_test.hpp"
#include "mge/asset/asset.hpp"

class test_asset : public mge::asset_test
{
protected:
    void SetUp() override
    {
        mge::properties p;
        p.set("directory", "./assets");
        mge::asset::mount("/", "file", p);
    }

    void TearDown() override { mge::asset::unmount("/"); }
};

TEST_F(test_asset, mount_unmount) {}

TEST_F(test_asset, red_jpg)
{
    using namespace mge::literals;
    mge::asset_ref a = std::make_shared<mge::asset>("/images/red.jpg");
    EXPECT_EQ(382u, a->size());
    EXPECT_EQ("image/jpeg"_at, a->type());
    EXPECT_EQ("image/jpeg"_at, a->type());
}