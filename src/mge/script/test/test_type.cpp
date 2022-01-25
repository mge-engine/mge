// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"
#include "test/googletest.hpp"

#include <string_view>

using namespace std::literals;

namespace mge {

    class test_type : public ::testing::Test
    {
    public:
        void SetUp() override {}
    };

    TEST_F(test_type, void_type)
    {
        auto t = mge::script::type<void>();
        EXPECT_EQ("void"sv, t.name());
        auto m = t.module();
        EXPECT_TRUE(m.is_root());
    }

    TEST_F(test_type, simple_types)
    {
        auto t1 = mge::script::type<void>();
        auto t2 = mge::script::type<char>();
        auto t3 = mge::script::type<unsigned char>();
        auto t4 = mge::script::type<signed char>();
        auto t5 = mge::script::type<short>();
        auto t6 = mge::script::type<unsigned short>();
        auto t7 = mge::script::type<int>();
        auto t8 = mge::script::type<unsigned int>();
    }

    enum enum1
    {
        E1,
        E2
    };

    TEST_F(test_type, enum_type)
    {
        using namespace mge::script;
        module("mge")(type<enum1>().enum_value("E1", E1).enum_value("E2", E2));
    }

    struct foo
    {
        int x;
        int y;
    };

    TEST_F(test_type, struct_type)
    {
        using namespace mge::script;
        module("mge")(type<foo>("foo"));
    }

    struct bar : public foo
    {};

    TEST_F(test_type, inheritance)
    {
        using namespace mge::script;
        module("mge")(type<foo>("foo"), type<bar>("bar").base(type<foo>()));
    }
#if 0 // will not compile
    struct nobar
    {};

    TEST_F(test_type, inheritance_fail)
    {
        using namespace mge::script;
        module("mge")(type<foo>("foo"), type<nobar>("nobar").base(type<foo>()));
    }
#endif

    struct constructed
    {
        constructed() {}
        constructed(int, int) {}
    };

    TEST_F(test_type, constructor)
    {
        using namespace mge::script;
        module("mge")(type<constructed>("constructed")
                          .constructor<void>()
                          .constructor<int, int>());
    }

    struct fields
    {
        int   x;
        float y;
    };

    TEST_F(test_type, fields)
    {
        using namespace mge::script;
        module("mge")(type<fields>("fields")
                          .field("x", &fields::x)
                          .field("y", &fields::y));
    }

} // namespace mge
