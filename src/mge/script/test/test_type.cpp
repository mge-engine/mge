// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/script/signature.hpp"
#include "mge/script/type.hpp"
#include "mge/script/type_details.hpp"

#include "test/googletest.hpp"

#include "mock_call_context.hpp"

#include <string_view>

using namespace std::literals;
using namespace testing;

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
        constructed() { ++constructed_count; }
        constructed(int, int) { ++constructed_count; }

        static int constructed_count;
    };

    int constructed::constructed_count = 0;

    TEST_F(test_type, constructor)
    {
        using namespace mge::script;
        module("mge")(type<constructed>("constructed")
                          .constructor()
                          .constructor<int, int>());

        type_details_ref r =
            type_details::get(std::type_index(typeid(constructed)));
        EXPECT_TRUE(r);
        class_type_details* ct = dynamic_cast<class_type_details*>(r.get());

        EXPECT_EQ(2u, ct->constructors().size());
        EXPECT_EQ(0u, ct->constructors()[0].signature.size());
        constructed check;
        constructed::constructed_count = 0;

        {
            MOCK_call_context ctx;
            EXPECT_CALL(ctx, this_ptr()).Times(1).WillOnce(Return(&check));
            EXPECT_TRUE(ct->constructors()[0].new_at);
            ct->constructors()[0].new_at(ctx);
            EXPECT_EQ(1, constructed::constructed_count);
        }

        std::shared_ptr<constructed>* shared;
        constructed::constructed_count = 0;
        {
            MOCK_call_context ctx;
            EXPECT_CALL(ctx, shared_ptr_address())
                .Times(1)
                .WillOnce(Return(&shared));
            ct->constructors()[0].new_shared(ctx);
            EXPECT_EQ(1, constructed::constructed_count);
        }
    }

    struct fields
    {
        fields()
            : x(123)
            , y(1213.33)
            , specific(4)
        {}

        int     x;
        float   y;
        int32_t specific;
    };

    TEST_F(test_type, fields)
    {
        using namespace mge::script;
        module("mge")(type<fields>("fields")
                          .constructor()
                          .field("x", &fields::x)
                          .field("y", &fields::y)
                          .field("specific", &fields::specific));

        type_details_ref r = type_details::get(std::type_index(typeid(fields)));
        EXPECT_TRUE(r);
        class_type_details* ct = dynamic_cast<class_type_details*>(r.get());

        std::shared_ptr<fields>* shared;
        {
            MOCK_call_context ctx;
            EXPECT_CALL(ctx, shared_ptr_address())
                .Times(1)
                .WillOnce(Return(&shared));
            ct->constructors().at(0).new_shared(ctx);
            EXPECT_EQ(123, (*shared)->x);
        }
        // set x
        {
            MOCK_call_context ctx;
            EXPECT_CALL(ctx, this_ptr())
                .Times(1)
                .WillOnce(Return(shared->get()));
            EXPECT_CALL(ctx, int32_t_parameter(_))
                .Times(1)
                .WillOnce(Return(4711));
            ct->fields().at(0).setter(ctx);
            EXPECT_EQ(4711, (*shared)->x);
        }
        // get x
        {
            ::testing::StrictMock<MOCK_call_context> ctx;
            EXPECT_CALL(ctx, this_ptr())
                .Times(1)
                .WillOnce(Return(shared->get()));
            EXPECT_CALL(ctx, store_int32_t_result(4711)).Times(1);
            ct->fields().at(0).getter(ctx);
        }
    }

    struct memberfunctions
    {
        enum K
        {
            AAA = 1,
            BBB = 2
        };

        int  foo(int, int) { return 0; }
        void void_foo(int) {}
        int  const_foo(int) const { return 0; }

        memberfunctions& set_field(K, bool) { return *this; }
    };

    TEST_F(test_type, memberfunction)
    {
        using namespace mge::script;
        module("mge")(type<memberfunctions>("memberfunctions")(
                          type<memberfunctions::K>("K").enum_value(
                              "AAA",
                              memberfunctions::K::AAA))
                          .method("foo", &memberfunctions::foo)
                          .method("void_foo", &memberfunctions::void_foo)
                          .method("const_foo", &memberfunctions::const_foo)
                          .method("set_field", &memberfunctions::set_field));
    }

} // namespace mge
