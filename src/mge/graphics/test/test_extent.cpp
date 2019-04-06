#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"
#include "mge/graphics/extent.hpp"

TEST(extent, print)
{
    mge::extent ex(640, 480);
    mge::test_stream_output(ex, "extent[width=640, height=480]");
}

TEST(extent, area)
{
    mge::extent ex(640, 480);
    EXPECT_EQ(640*480, ex.area());
}
