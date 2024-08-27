// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "boost/boost_preprocessor.hpp"
#include "mge/core/call_debugger.hpp"
#include "mge/script/invocation_context.hpp"
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

class base
{
public:
    base() {}
    base(const base&) = delete;
    virtual ~base() {}
    virtual int f() { return 42; }
};

class script_impl : public mge::script::proxy<base>
{
public:
    script_impl() {}

    virtual ~script_impl() {}

    virtual int f() { return m_context->call<int>("f"); };
};

TEST(type, proxy)
{
    using namespace mge::script;

    mge::script::module m;
    m(type<base>().proxy<script_impl>());
};
