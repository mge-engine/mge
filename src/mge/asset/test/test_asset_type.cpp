#include "test/googletest.hpp"
#include "mge/asset/asset_type.hpp"

using namespace mge::string_literals;

TEST(asset_type, literal)
{
    auto t = "image/jpeg"_asset_type;
    EXPECT_STREQ("image", t.type().c_str());
    EXPECT_STREQ("jpeg", t.subtype().c_str());
}
