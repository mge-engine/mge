#include "mge/math/aabox.hpp"
#include "test/googletest.hpp"
using namespace mge;

TEST(aabox, create)
{
    aabox b (fvec3({2,3,4}), fvec3{1, 4, 2});

    EXPECT_EQ(fvec3({1,3,2}), b.min());
    EXPECT_EQ(fvec3({2,4,4}), b.max());
}

TEST(aabox, print)
{
    aabox b (fvec3({2,3,4}), fvec3{1, 4, 2});
    std::stringstream ss;
    ss << b;
    EXPECT_STREQ("aabox[min=[1, 3, 2], max=[2, 4, 4]]", ss.str().c_str());
}

TEST(aabox, contains)
{
    aabox b (fvec3({2,3,4}), fvec3{1, 4, 2});
    fvec3 v(1.5, 3.5, 2.5);
    EXPECT_TRUE(b.contains(v));
}
