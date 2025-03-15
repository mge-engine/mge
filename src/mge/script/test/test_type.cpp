// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "boost/boost_preprocessor.hpp"
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
    test_class()
    {
        test_class_ctor_called = true;
    }
    test_class(const test_class&) = delete;
    ~test_class()
    {
        test_class_dtor_called = true;
    }
};

TEST(type, ctor_dtor)
{
    using namespace mge::script;

    mge::script::module m;
    m(type<test_class>());

    const auto& t = m.type("test_class");
    EXPECT_EQ("test_class", t.name());
    EXPECT_TRUE(t.class_specific().destroy);
    EXPECT_EQ(1, t.class_specific().size);
    EXPECT_EQ(1, t.class_specific().constructors.size());
    EXPECT_EQ(1, t.class_specific().make_shared_constructors.size());

    MOCK_call_context ctx;
    void*             data = mge::malloc(1);
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
    EXPECT_CALL(ctx, this_ptr()).WillRepeatedly(Return(data));
    t.class_specific().constructors[0].second(ctx);
    EXPECT_TRUE(test_class_ctor_called);
    test_class_ctor_called = false;
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
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
    virtual int f()
    {
        return 42;
    }
};

class script_impl : public mge::script::proxy<base>
{
public:
    script_impl() {}

    virtual ~script_impl() {}

    virtual int f()
    {
        return m_context->call<int>("f");
    };
};

TEST(type, proxy)
{
    using namespace mge::script;

    mge::script::module m;
    m(type<base>().proxy<script_impl>());
};

class overloaded_methods
{
public:
    overloaded_methods() = default;
    overloaded_methods(const overloaded_methods&) = delete;
    overloaded_methods& operator=(const overloaded_methods&) = delete;
    ~overloaded_methods() = default;

    int foobar(int x)
    {
        return x;
    }
    int foobar(float x)
    {
        return static_cast<int>(-x);
    }
};

TEST(type, overloaded_methods)
{
    using namespace mge::script;

    mge::script::module m;
    m(type<overloaded_methods>()
          .method("foobar",
                  static_cast<int (overloaded_methods::*)(int)>(
                      &overloaded_methods::foobar))
          .method("foobar",
                  static_cast<int (overloaded_methods::*)(float)>(
                      &overloaded_methods::foobar)));

    const auto& t = m.type("overloaded_methods");
    const auto& methods = t.class_specific().methods;

    EXPECT_EQ(2, methods.size());
    overloaded_methods om;
    MOCK_call_context  ctx;
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
    EXPECT_CALL(ctx, this_ptr()).WillRepeatedly(Return(&om));
    EXPECT_CALL(ctx, int32_t_result(42)).Times(1);
    EXPECT_CALL(ctx, int32_t_parameter(0)).WillOnce(Return(42));
    std::get<3>(methods[0])(ctx);
    EXPECT_CALL(ctx, before_call()).Times(1);
    EXPECT_CALL(ctx, after_call()).Times(1);
    EXPECT_CALL(ctx, int32_t_result(-42)).Times(1);
    EXPECT_CALL(ctx, float_parameter(0)).WillOnce(Return(42.0f));
    std::get<3>(methods[1])(ctx);
}

TEST(type, no_alias_on_special)
{
    using namespace mge::script;

    try {
        mge::script::module test_module("test");
        test_module(type<std::string>("string_alias"));
        FAIL() << "Expected exception";
    } catch (const mge::exception& e) {
        EXPECT_THAT(e.what(),
                    HasSubstr("is a special type and cannot be aliased"));
    }
}

TEST(type, string)
{
    using namespace mge::script;

    auto t = type<std::string>();
    EXPECT_TRUE(t.is_string());
}

class abstract_class
{
public:
    virtual ~abstract_class() = default;
    virtual void f() = 0;
};

TEST(type, abstract)
{
    using namespace mge::script;

    auto t = type<abstract_class>();
    EXPECT_TRUE(t.is_abstract());
}
