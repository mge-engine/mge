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