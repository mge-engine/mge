// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/type_name.hpp"
#include <memory>

class FooBar
{};
struct WutzWutz
{};

enum class Enumi
{
    FOO
};

namespace NameSpace {
    class FooInSpace {};
}

TEST(type_name, class_type_name)
{
    std::string name(mge::type_name<FooBar>());
    EXPECT_STREQ("FooBar", name.c_str());
}

TEST(type_name, shared_ptr_struct_type_name)
{
    std::string name(mge::type_name<std::shared_ptr<WutzWutz>>());
    EXPECT_STREQ("std::shared_ptr<WutzWutz>", name.c_str());
}

TEST(type_name, struct_type_name)
{
    std::string name(mge::type_name<WutzWutz>());
    EXPECT_STREQ("WutzWutz", name.c_str());
}


TEST(type_name, enum_class_type_name)
{
    std::string name(mge::type_name<Enumi>());
    EXPECT_STREQ("Enumi", name.c_str());
}

TEST(type_name, pod_type_name)
{
    std::string name(mge::type_name<int>());
    EXPECT_STREQ("int", name.c_str());
}

TEST(type_name, pointer_type_name)
{
    std::string name(mge::type_name<FooBar**>());
    EXPECT_STREQ("FooBar**", name.c_str());

}

TEST(type_name, namespace_name)
{
    std::string name(mge::namespace_name<NameSpace::FooInSpace>());
    EXPECT_STREQ("NameSpace", name.c_str());
}

TEST(type_name, empty_namespace_name)
{
    std::string name(mge::namespace_name<int>());
    EXPECT_STREQ("", name.c_str());
}

TEST(type_name, base_type_name)
{
    std::string name(mge::base_type_name<NameSpace::FooInSpace>());
    EXPECT_STREQ("FooInSpace", name.c_str());
}
