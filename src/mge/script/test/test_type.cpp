// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/call_debugger.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"
#include "mock_call_context.hpp"
#include "test/googletest.hpp"

#include <typeinfo>

using namespace testing;

static bool test_class_ctor_called{false};
static bool test_class_dtor_called{false};

struct test_class
{
    test_class() { test_class_ctor_called = true; }
    test_class(const test_class&) = delete;
    ~test_class() { test_class_dtor_called = true; }
};

TEST(type, ctor_dtor)
{
    using namespace mge::script;

    mge::script::module m;
    m(type<test_class>());

    const auto& t = m.type("test_class");
    EXPECT_EQ("test_class", t.name());
    EXPECT_TRUE(t.class_specific().default_construct);
    EXPECT_TRUE(t.class_specific().destroy);
    EXPECT_EQ(1, t.class_specific().size);

    MOCK_call_context ctx;
    void*             data = mge::malloc(1);

    EXPECT_CALL(ctx, get_this()).WillRepeatedly(Return(data));
    t.class_specific().default_construct(ctx);
    EXPECT_TRUE(test_class_ctor_called);
    test_class_ctor_called = false;
    t.class_specific().destroy(ctx);
    EXPECT_TRUE(test_class_dtor_called);
    test_class_dtor_called = false;
    mge::free(data);
}

struct test_fields
{
    int       x;
    const int y;
};

TEST(type, fields)
{
    using namespace mge::script;

    mge::script::module m;
    m(type<test_fields>()
          .field("x", &test_fields::x)
          .field("y", &test_fields::y));

    const auto& t = m.type("test_fields");
    const auto& fields = t.class_specific().fields;
    EXPECT_EQ(2, fields.size());
    auto const_setter = std::get<3>(fields[1]);
    EXPECT_FALSE(const_setter);
};

class c1
{
public:
    c1() = default;
    virtual ~c1() = default;

    virtual int f1() { return 42; }
};

class c2 : public c1
{
public:
    c2() = default;
    virtual ~c2() = default;

    virtual int f1() override { return 666; }
};

TEST(type, virtual_functions)
{
    auto member_function = &c1::f1;
    auto closure = [member_function](c1* obj) {
        return (obj->*member_function)();
    };

    c2 obj;
    EXPECT_EQ(666, closure(&obj));
}
