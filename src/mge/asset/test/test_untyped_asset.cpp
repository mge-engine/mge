#include "test/googletest.hpp"
#include "mge/asset/untyped_asset.hpp"

TEST(untyped_asset, create)
{
    mge::path sample_path("test");
    mge::untyped_asset a(sample_path);
    EXPECT_EQ(sample_path, a.path());
}
