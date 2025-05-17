// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "asset_test.hpp"
#include "mge/asset/asset.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/image.hpp"

class test_asset : public mge::asset_test
{
protected:
    void SetUp() override
    {
        mge::properties p;
        p.set("directory", "./assets");
        mge::asset::mount("/", "file", p);
    }

    void TearDown() override
    {
        mge::asset::unmount("/");
    }
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

TEST_F(test_asset, red_jpg_load)
{
    using namespace mge::literals;
    mge::asset_ref a = std::make_shared<mge::asset>("/images/red.jpg");
    auto           load_result = a->load();
    EXPECT_STREQ("class std::shared_ptr<class mge::image>",
                 load_result.type().name());
    mge::image_ref image = std::any_cast<mge::image_ref>(load_result);
    EXPECT_TRUE(image);
    EXPECT_EQ(100, image->extent().width);
    EXPECT_EQ(100, image->extent().height);
}

TEST_F(test_asset, triangle_frag_spv)
{
    using namespace mge::literals;
    mge::asset_ref a =
        std::make_shared<mge::asset>("/shaders/triangle.frag.spv");
    auto load_result = a->load();
    EXPECT_STREQ(typeid(std::shared_ptr<mge::buffer>).name(),
                 load_result.type().name());
}

TEST_F(test_asset, teapot_obj)
{
    using namespace mge::literals;
    mge::asset_ref a = std::make_shared<mge::asset>("/models/teapot.obj");
    EXPECT_EQ("model/obj"_at, a->type());
    auto load_result = a->load();
}