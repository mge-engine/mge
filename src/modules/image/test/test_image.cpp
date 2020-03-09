#include "test/googletest.hpp"
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_loader.hpp"
#include "mge/graphics/image.hpp"

using namespace testing;
using namespace mge::string_literals;
using namespace std::string_literals;

TEST(image, load_tga)
{
    auto imageasset = "images/blue.tga"_asset;
    auto loader = mge::asset_loader::loader(imageasset);
    EXPECT_TRUE(loader);
    auto img = loader->load(imageasset);
    mge::image_ref imgref = std::any_cast<mge::image_ref>(img);
    EXPECT_EQ(128, imgref->extent().width());
}

TEST(image, load_gif)
{
    auto imageasset = "images/green.gif"_asset;
    auto loader = mge::asset_loader::loader(imageasset);
    EXPECT_TRUE(loader);
    auto img = loader->load(imageasset);
    mge::image_ref imgref = std::any_cast<mge::image_ref>(img);
    EXPECT_EQ(128, imgref->extent().width());
}

TEST(image, load_png)
{
    auto imageasset = "images/green.png"_asset;
    auto loader = mge::asset_loader::loader(imageasset);
    EXPECT_TRUE(loader);
    auto img = loader->load(imageasset);
    mge::image_ref imgref = std::any_cast<mge::image_ref>(img);
    EXPECT_EQ(128, imgref->extent().width());
}

TEST(image, load_bmp)
{
    auto imageasset = "images/green.bmp"_asset;
    auto loader = mge::asset_loader::loader(imageasset);
    EXPECT_TRUE(loader);
    auto img = loader->load(imageasset);
    mge::image_ref imgref = std::any_cast<mge::image_ref>(img);
    EXPECT_EQ(128, imgref->extent().width());
}

