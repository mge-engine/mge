// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_loader.hpp"
#include "mge/graphics/image.hpp"
#include "test/googletest.hpp"

using namespace testing;
using namespace mge::string_literals;
using namespace std::string_literals;

TEST(jpeg, load)
{
    auto imageasset = "images/red.jpg"_asset;
    auto loader     = mge::asset_loader::loader(imageasset);
    EXPECT_TRUE(loader);
    auto           img    = loader->load(imageasset);
    mge::image_ref imgref = std::any_cast<mge::image_ref>(img);
    EXPECT_EQ(100, imgref->extent().width());
}
