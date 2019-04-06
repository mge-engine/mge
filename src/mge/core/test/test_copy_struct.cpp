#include "test/googletest.hpp"
#include "mge/core/copy_struct.hpp"

struct some_struct {
    int int_value;
    char char_value;
    double double_value;
};

TEST(copy_struct, to_reference)
{
    some_struct v1{123, 'A', 3.14159265};
    some_struct v2;
    mge::copy_struct(v2, v1);
    EXPECT_TRUE(memcmp(&v2, &v1, sizeof(some_struct))==0);
}

TEST(copy_struct, to_pointer)
{
    some_struct v1{123, 'A', 3.14159265};
    some_struct v2;
    mge::copy_struct(&v2, v1);
    EXPECT_TRUE(memcmp(&v2, &v1, sizeof(some_struct))==0);
}
