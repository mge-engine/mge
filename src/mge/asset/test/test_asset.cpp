#include "test/googletest.hpp"
#include "mge/asset/asset.hpp"

using namespace mge;

TEST(asset, create)
{
    mge::path sample_path("test");
    mge::asset a(sample_path);
    EXPECT_EQ(sample_path, a.path());
}

TEST(asset, literal)
{
    auto a = "test"_asset;
    mge::path sample_path("test");
    EXPECT_EQ(sample_path, a.path());
}
