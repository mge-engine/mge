#include "mge/graphics/memory_image.hpp"
#include "mge/graphics/texture_type.hpp"
#include "mock_render_context.hpp"
#include "mock_texture.hpp"
#include "test/googletest.hpp"

using namespace testing;

TEST(texture, set_data)
{
    auto              ctx = std::make_shared<MOCK_render_context>();
    MOCK_texture      tex(*ctx, mge::texture_type::TYPE_2D);
    mge::extent       ext(1024, 1024);
    mge::memory_image img(mge::image_format::RGBA_BYTE, ext);
    EXPECT_CALL(tex,
                set_data(mge::image_format::RGBA_BYTE,
                         ext,
                         img.data(),
                         img.binary_size()))
        .Times(1);
    tex.set_data(img);
}