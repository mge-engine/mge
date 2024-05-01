#include "mge/graphics/image_format.hpp"
#include "test/googletest.hpp"

using namespace testing;

TEST(image_format, components)
{
    mge::image_format f(mge::image_format::data_format::RGB,
                        mge::data_type::FLOAT);
    EXPECT_EQ(3, f.components());
    f = mge::image_format(mge::image_format::data_format::RGBA,
                          mge::data_type::FLOAT);
    EXPECT_EQ(4, f.components());
}