#include "mge/script/type.hpp"
#include "test/googletest.hpp"

TEST(type_identifier, pointer)
{
    auto tid = mge::script::make_type_identifier<int*>();

    EXPECT_STRNE(typeid(int).name(), tid.type_index().name());
}

TEST(type_identifier, reference)
{
    auto tid = mge::script::make_type_identifier<int&>();

    EXPECT_STREQ(typeid(int).name(), tid.type_index().name());
}

TEST(type_identifier, lvalue_reference)
{
    auto tid = mge::script::make_type_identifier<int&&>();

    EXPECT_STREQ(typeid(int).name(), tid.type_index().name());
}
