// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "asset_test.hpp"
#include "mge/asset/asset.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/image.hpp"
#include <filesystem>

class test_asset : public mge::asset_test
{
protected:
    void SetUp() override
    {
        mge::properties p;
        p.set("directory", "./assets");
        mge::asset::mount("/", "file", p);

        mge::properties p_temp;
        p_temp.set("directory",
                   std::filesystem::temp_directory_path().string());
        mge::asset::mount("/temp",
                          "file",
                          mge::asset_source::access_mode::READ_WRITE,
                          p_temp);
    }

    void TearDown() override
    {
        mge::asset::unmount("/temp");
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
}

TEST_F(test_asset, red_jpg_load)
{
    using namespace mge::literals;
    mge::asset_ref a = std::make_shared<mge::asset>("/images/red.jpg");
    auto           load_result = a->load();
#ifdef MGE_COMPILER_MSVC
    EXPECT_STREQ("class std::shared_ptr<class mge::image>",
                 load_result.type().name());
#endif
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
    auto          load_result = a->load();
    mge::mesh_ref mesh = std::any_cast<mge::mesh_ref>(load_result);
}

TEST_F(test_asset, store_and_load_image)
{
    using namespace mge::literals;

    // Load original image
    mge::asset_ref original = std::make_shared<mge::asset>("/images/red.jpg");
    auto           load_result = original->load();
    mge::image_ref original_image = std::any_cast<mge::image_ref>(load_result);
    EXPECT_TRUE(original_image);
    EXPECT_EQ(100, original_image->extent().width);
    EXPECT_EQ(100, original_image->extent().height);

    // Store image as PNG (loaded JPEGs are RGBA format)
    mge::asset_ref stored = std::make_shared<mge::asset>("/temp/test_red.png");
    stored->store("image/png"_at, original_image);

    // Load stored image
    mge::asset_ref reloaded =
        std::make_shared<mge::asset>("/temp/test_red.png");
    EXPECT_TRUE(reloaded->exists());
    EXPECT_EQ("image/png"_at, reloaded->type());

    auto           reload_result = reloaded->load();
    mge::image_ref reloaded_image =
        std::any_cast<mge::image_ref>(reload_result);
    EXPECT_TRUE(reloaded_image);
    EXPECT_EQ(100, reloaded_image->extent().width);
    EXPECT_EQ(100, reloaded_image->extent().height);
    EXPECT_EQ(original_image->format().format(),
              reloaded_image->format().format());
    EXPECT_EQ(original_image->format().type(), reloaded_image->format().type());

    // Clean up
    std::filesystem::remove(std::filesystem::temp_directory_path() /
                            "test_red.png");
}