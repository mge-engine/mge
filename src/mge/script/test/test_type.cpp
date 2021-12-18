// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/bind.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"
#include "test/googletest.hpp"

#include <string_view>

using namespace std::literals;

namespace mge {

    class test_type : public ::testing::Test
    {
    public:
        void SetUp() override { mge::script::bind_all(); }
    };

    TEST_F(test_type, void_type)
    {
        auto t = mge::script::type<void>();
        EXPECT_EQ("void"sv, t.name());
        auto m = t.module();
        EXPECT_TRUE(m.is_root());
    }

    struct foo
    {
        int x;
        int y;
    };

    TEST_F(test_type, struct_type)
    {
        using namespace mge::script;
        module("mge")(type<foo>().field("x", &foo::x));
    }

    enum enum1
    {
        E1,
        E2
    };

    TEST_F(test_type, enum_type)
    {
        using namespace mge::script;
        module("mge")(type<enum1>());
    }

} // namespace mge
