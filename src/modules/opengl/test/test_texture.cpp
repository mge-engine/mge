// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "opengl_test.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/texture_2d.hpp"
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_loader.hpp"

using namespace mge::string_literals;
using namespace std::string_literals;


namespace opengl {
    class test_texture : public opengltest_with_window
    {};

    TEST_F(test_texture, create)
    {
        auto texture = window->render_context()
            .create_texture_2d();
        EXPECT_TRUE(texture.get() != nullptr);
    }

    TEST_F(test_texture, create_mapped)
    {
        auto imageasset = "images/green.png"_asset;
        auto loader = mge::asset_loader::loader(imageasset);
        EXPECT_TRUE(loader);
        auto i = loader->load(imageasset);
        mge::image_ref img = std::any_cast<mge::image_ref>(i);

        auto texture = window->render_context()
            .create_texture_2d(img);
        EXPECT_TRUE(texture.get() != nullptr);

    }
}
